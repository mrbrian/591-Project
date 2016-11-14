#include "scene.h"
#include "misc.h"

#define MAX_DEPTH   5
#define BG_COLOR    Color(0,0,0)

Light::Light(Point3D pos, Color a, Color d, Color s, double in_watts)
{
    position = pos;
    Ia = a;
    Id = d;
    Is = s;
    watts = in_watts;
}

void Light::emit_photons(int to_emit, vector<photon*> *out_photons)
{
    int num_emit = 0;
    while (num_emit < to_emit)
    {
        double x;
        double y;
        double z;
        Vector3D dir;
        do
        {
            x = m_RND_1;
            y = m_RND_1;
            z = m_RND_1;
            dir = Vector3D(x, y, z);
        }
        while (dir.length2() > 1);
        photon *p = new photon;
        p->set_position(position);
        dir.normalize();
        p->set_direction(dir);
        p->set_color(Id);       // only using the diffuse color..(?)

        out_photons->push_back(p);
        num_emit++;
    }
}

// ----------------------------------------------------------------------------
// BuildOrthonormalSystem ()
//
// Generating outgoing ray directions by uniform sampling on a hemisphere
//
// Input: vectors v1 and v2
// Output: vector v3 so (v1, v2, v3) form an orthonormal system
//         (assuming v1 is already normalized)
//
// ----------------------------------------------------------------------------
void BuildOrthonormalSystem(const Vector3D& v1, Vector3D& v2, Vector3D& v3)
{
    float inverse_length, den;

    if (std::abs(v1[0]) > std::abs(v1[1]))
    {
        // project to the plane y = 0
        // construct a normalized orthogonal vector on this plane
        den = sqrtf(v1[0] * v1[0] + v1[2] * v1[2]);
        inverse_length = 1.f / den;
        v2 = Vector3D(-v1[2] * inverse_length, 0.0f, v1[0] * inverse_length);
    }
    else
    {
        // project to the plane x = 0
        // construct a normalized orthogonal vector on this plane
        den = sqrtf(v1[1] * v1[1] + v1[2] * v1[2]);
        inverse_length = 1.0f / den;
        v2 = Vector3D(0.0f, v1[2] * inverse_length, -v1[1] * inverse_length);
    }

    // construct v3 as the cross-product between v1 and v2
    v3 = v1.cross(v2);
}

// ----------------------------------------------------------------------------
// HemisphereSampling ()
//
// Generating outgoing ray directions by uniform sampling on a hemisphere
//
// Input: normal vector at the intersection point
// Output: outgoing ray direction from uniform sampling on a hemisphere
//
// ----------------------------------------------------------------------------

Vector3D HemisphereSampling(Vector3D m_normal)
{
    float r_1 = m_RND_2;
    float r_2 = m_RND_2;

    float r = sqrt(1 - r_1 * r_2);
    float phi = 2 * M_PI * r_2;

    double vx = cos(phi) * r;
    double vy = sin(phi) * r;
    double vz = r_1;

    Vector3D sampled_ray_direction = Vector3D(vx, vy, vz);

    // Now we build an otrhotnormal frame system
    // to "rotate" the sampled ray to this system
    Vector3D v2, v3;
    BuildOrthonormalSystem (m_normal, v2, v3);

    // Construct the normalized rotated vector
    double vecx = Vector3D(v2[0], v3[0], m_normal[0]).dot(sampled_ray_direction);
    double vecy = Vector3D(v2[1], v3[1], m_normal[1]).dot(sampled_ray_direction);
    double vecz = Vector3D(v2[2], v3[2], m_normal[2]).dot(sampled_ray_direction);
    Vector3D m_rotated_ray_direction = Vector3D(vecx,vecy,vecz);

    m_rotated_ray_direction.normalize();
    return m_rotated_ray_direction;
}


Color *Scene::Render()
{
    Color * result = new Color[cam.imgWidth * cam.imgHeight];
    // iterate over the pixels & set colour values
    for (int x = 0; x < cam.imgWidth; x++)
    {
        for (int y = 0; y < cam.imgHeight; y++)
        {
            // determine ray vector
            Point3D p = imgPlane->ImageToWorldSpace(x, y, cam.imgWidth, cam.imgHeight);
            Vector3D v = p - cam.position;
            v.normalize();

            Color &c = result[x + y * cam.imgWidth];

            if (!trace_ray(cam.position, v, &c, 1))   // if ray hit nothing
                c = BG_COLOR;                               // use background color
        }
    }
    return result;
}

// check if objects lay between a point and light position
bool intersection_test(Scene *scene, Point3D o, Point3D light_pos)
{
    Vector3D v = light_pos - o;
    double dist = v.length();
    v.normalize();

    float tmin = INFINITY;

    for(std::vector<SceneObject*>::iterator it = scene->objects.begin(); it != scene->objects.end(); ++it)
    {
        SceneObject *object= (*it);

        Vector3D n;
        double t = object->intersect(o, v, &n);      // whats the n?    -1 if no hit.

        if (0 <= t && t <= tmin)
            tmin = t;
    }
    if (tmin > dist)
        return false;
    if (tmin < 0)
        return false;

    return true;
}

bool Scene::trace_ray(Point3D o, Vector3D v, Color *color, int depth)
{
    if (depth > MAX_DEPTH)  // stop recursing
        return false;

    double t_min = INFINITY;
    Vector3D n_min;
    SceneObject *hitObject = NULL;
    v.normalize();

    Color &col = *color;
    if (depth == 1)         // start ... with no colour
        col = Color(0,0,0);

    for(std::vector<SceneObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
    {
        SceneObject *obj = (*it);

        Vector3D n;
        double t = obj->intersect(o, v, &n);      // whats the n?

        if (0 <= t && t < t_min)
        {
            t_min = t;
            if (n.dot(v) >= 0)
                n = -n;
            n_min = n;
            hitObject = obj;
        }
    }

    if (hitObject == NULL)              // check for no intersection
    {
        return false;
    }

    Point3D p_int = o + t_min * v;      // calculate intersection point

    p_int = p_int + 0.001 * n_min;      // pull back point a bit, avoid self intersection

    // found closest intersection
    Point3D p = o + t_min * v;

    for(std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
    {
        Light *light= (*it);
        col = col + hitObject->material->Ka * light->Ia;

        Point3D new_p = p + 0.1 * (light->position - p);
        if (intersection_test(this, new_p, light->position))
           continue;

        Vector3D n = n_min;
        Vector3D l = light->position - p;
        l.normalize();
        Vector3D r = -l + 2 * (l.dot(n)) * n;

        // add diffuse color
        col = col + (hitObject->material->GetKd(p) * fmax(n.dot(l), 0) * light->Id);
        // add specular color
        col = col + (hitObject->material->Ks * pow(fmax(r.dot(-v), 0), hitObject->material->p) * light->Is);
    }

    Color reflect_rgb;     // reflection color;

    // calculate reflect vector
    Vector3D r = v + (2 * n_min.dot(-v)) * n_min;

    trace_ray(p_int, r, &reflect_rgb, depth + 1);
    // add reflection color
    col = col + reflect_rgb * hitObject->material->Kr;
    return true;
}


SceneObject *find_closest_intersection(Scene *scene, Point3D o, Vector3D v, double *t_min_ptr, Vector3D *n_min_ptr)
{
	SceneObject *hitObject = NULL;
	double &t_min = *t_min_ptr;
	Vector3D &n_min = *n_min_ptr;

	for (std::vector<SceneObject*>::iterator it = scene->objects.begin(); it != scene->objects.end(); ++it)
	{
		SceneObject *obj = (*it);

		Vector3D n;
		double t = obj->intersect(o, v, &n);      // whats the n?

		if (0 <= t && t < t_min)
		{
			t_min = t;
			if (n.dot(v) >= 0)
				n = -n;
			n_min = n;
			hitObject = obj;
		}
	}
	return hitObject;
}

bool Scene::trace_primary_ray(Point3D in_pos, Vector3D in_dir, Color *in_clr, Point3D *_out_pos, Vector3D *_out_norm, Vector3D *_out_reflect, Vector3D *_out_refract, Color *_out_clr, Material *_out_mat)
{
    Point3D &out_pos = *_out_pos;
    Vector3D &out_norm = *_out_norm;
    Vector3D &out_reflect = *_out_reflect;
    Vector3D &out_refract = *_out_refract;
    Color &out_clr = *_out_clr;
    Material &out_mat = *_out_mat;

	double t_min = INFINITY;
	Vector3D n_min;
	SceneObject *hitObject = NULL;
    in_dir.normalize();

	// find closest intersection point, the object
    hitObject = find_closest_intersection(this, in_pos, in_dir, &t_min, &n_min);

	if (hitObject == NULL)              // check for no intersection
        return false;

    out_mat = *hitObject->material;

    out_pos = in_pos + t_min * in_dir;      // calculate intersection point
    //p_int = p_int + 0.001 * n_min;      // pull back point a bit, avoid self intersection

    out_norm = n_min;
    out_reflect = in_dir + (2 * n_min.dot(-in_dir)) * n_min;

    // add diffuse color    
    //out_clr = (hitObject->material->GetKd(p_int));//* fmax(n.dot(in_dir), 0) * light->Id);
    return true;
}


// collide photon with the scene objects
void Scene::trace_photon(photon *in_pho, int depth, vector<photon*> *out_list)
{
    // If depth >= MAX_DEPTH then Each recursive step will stop w/ a probability of 0.1
    if ((depth >= MAX_DEPTH) & (m_RND_2 <= 0.1))
        return;

    Point3D start_pos = Point3D(in_pho->x, in_pho->y, in_pho->z);
    Vector3D direction = in_pho->get_direction();
    Color *clr = in_pho->get_color();

    Point3D i_point;
    Vector3D i_normal;
    Vector3D i_reflect;
    Vector3D i_refract;
    Color i_clr;
    Material i_mat;

    if (!trace_primary_ray(start_pos, direction, clr, &i_point, &i_normal, &i_reflect, &i_refract, &i_clr, &i_mat))
    {
        printf("miss %d: %f %f %f - %f %f %f\n", depth, (start_pos)[0], (start_pos)[1], (start_pos)[2], direction[0], direction[1], direction[2]);
        return;
    }

    printf("%d: %f %f %f - %f %f %f - %f %f %f\n", depth, (start_pos)[0], (start_pos)[1], (start_pos)[2],
            direction[0], direction[1], direction[2],
            (*clr).R(), (*clr).G(), (*clr).B()
            );
    RayType ray_type = russian_roulette(&i_mat);

    if (ray_type == RayType::Diffuse)
        i_clr = Color(i_clr.R() * i_mat.Kd.R(), i_clr.G() * i_mat.Kd.G(), i_clr.B() * i_mat.Kd.B());

    bounce_photon(ray_type, &i_point, &i_normal, &i_reflect, &i_refract, &i_clr, depth, out_list);

    /*
    // subtract energy depending on absorption
    in_pho->p[3] -= 1;

    _Photon *pho_out = new _Photon();
    if (pho_out != NULL)
    {
        //store new photon splat
        photon_map->push_back(pho_out);
    }
    RayType next_ray = roulette(mat->Kd, mat->Ks);

    switch (next_ray)
    {
    case Diffuse:
        //  choose random hemisphere direction
    break;
    case Specular:
        // choose a reflect direction
    break;
    case Transmission:
        // check if transparent
        // if not then nothin
        return NULL;
    }
    return pho_out;

    --------

    // Russian Roulette (rr) strategy
    // If depth >= 5
    // then Each recursive step will stop w/ a probability of 0.1
    double rr_factor = 1.0;
    if ((depth >= 5) & (m_RND_2 <= 0.1)) return;
    rr_factor = 1.0 / (1.0 - 0.1);

    // Find ray intsersection with the scene
    m_Intersection intersection = scene.intersect(ray);
    if (!intersection) return;

    // Compute intersection hit point position and its normal
    m_Vector hit_point = ray.origin + ray.direction * intersection.t;
    m_Vector normal_at_hit_point = intersection.object->normal(hit_point);
    ray.origin = hit_point;

    // Add the emission to the color, scaled e/ the Russian Roulette probability weight.
    const double emission = intersection.object->emission;
    color = color + m_Vector(emission, emission, emission) * rr_factor;

    double ktot = intersection.object->kd + intersection.object->ks + intersection.object->kr;
    double m_random_float = m_RND_2;

    if (m_random_float < intersection.object->kd) // send a diffuse ray
    {
        ray.direction = HemisphereSampling(normal_at_hit_point);

        double cosine_t = ray.direction.DotProduct(normal_at_hit_point);
        m_Vector tmp;
        m_PathTracer(ray, depth+1, tmp);
        color = color + (tmp.Multiply(intersection.object->color)) * cosine_t * 0.1 * rr_factor;
    }
    else
        if (m_random_float < (intersection.object->kd + intersection.object->ks)) // send a specular ray
        {

            double cosine_t = ray.direction.DotProduct(normal_at_hit_point);
            ray.direction = (ray.direction - normal_at_hit_point*(2*cosine_t)).Normalize();
            m_Vector tmp_color = m_Vector(0,0,0);
            m_PathTracer(ray, depth+1, tmp_color);
            color = color + tmp_color * rr_factor;
        }
        else // send a transmission (refraction) ray
        {
            ray.direction = ProcessTransmissionRay (normal_at_hit_point, ray.direction);
            m_Vector tmp_color;
            m_PathTracer(ray, depth+1, tmp_color);
            color = color + tmp_color * 1.15 * rr_factor;
        }
    ----------
    */
}

void Scene::emit_photons(int num_photons)
{
    vector<photon*> photons;
    vector<photon*> out_photons;

    initialize_photons(num_photons, &photons);    // spawn initial photons for all lights

    for(std::vector<photon*>::iterator it = photons.begin(); it != photons.end(); ++it)
    {
        photon *obj = (*it);
        trace_photon(obj, 0, &out_photons);
    }
}

void Scene::bounce_photon(RayType ray_type, Point3D *i_pos, Vector3D *i_normal, Vector3D *i_reflect, Vector3D *i_refract, Color *i_clr, int depth, vector<photon*> *out_list)
{
    photon *new_photon;
    Vector3D new_dir;
    Point3D new_pos;

    switch (ray_type)
    {
    case Diffuse:
        //  choose random hemisphere direction
        new_pos = (*i_pos) + 0.001 * (*i_normal);      // pull back point a bit, avoid self intersection
        new_dir = HemisphereSampling(*i_normal);
    break;
    case Specular:
        // choose a reflect direction
        // calculate reflect vector
        new_pos = (*i_pos) + 0.001 * (*i_normal);      // pull back point a bit, avoid self intersection
        new_dir = *i_reflect;

    break;
    case Transmission:
        // check if transparent
        // if not then nothin
        new_pos = (*i_pos) - 0.001 * (*i_normal);      // pull back point a bit, avoid self intersection
        new_dir = *i_refract;
        return;
    }
    new_photon = new photon();
    new_photon->set_position(new_pos);
    new_photon->set_direction(new_dir);
    new_photon->set_color(*i_clr);

    trace_photon(new_photon, depth + 1, out_list);
}

Scene::RayType Scene::russian_roulette(Material *mat)   // [0, d] diffuse reflection ray, [d,s+d] specular ray, [s+d, 1] absorption
{
    Color diff = mat->Kd;
    Color spec = mat->Ks;

    double p_refl = std::max(diff.R() + spec.R(), diff.G() + spec.G());
    p_refl = std::max(p_refl, diff.B() + spec.B());

    float diff_sum = diff.R() + diff.G() + diff.B();
    float spec_sum = spec.R() + spec.G() + spec.B();

    float p_diff = diff_sum / (diff_sum + spec_sum) * p_refl;
    float p_spec = spec_sum / (diff_sum + spec_sum) * p_refl;

    float r = m_RND_2;

    if (r >= 0 && r < p_diff)
        return RayType::Diffuse;
    if (r >= p_diff && r < p_spec)
        return RayType::Specular;
    return RayType::Transmission;
}

void Scene::initialize_photons(int num_photons, vector<photon*> *out_photons)
{
    double total_watts = 0;
    double energy_per_photon = 0;

    for(std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
    {
        Light *obj = (*it);
        total_watts += obj->watts;
    }

    energy_per_photon = total_watts / num_photons;

    for(std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
    {
        Light *obj = (*it);
        int num_to_emit = (int)(obj->watts / energy_per_photon);
        obj->emit_photons(num_to_emit, out_photons);
    }
}
