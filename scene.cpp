#include "scene.h"
#include "misc.h"

#define MAX_DEPTH   5
#define BG_COLOR    Color(0,0,0)

// -----------------------------------------
//    RANDOM NUMBER GENERATION
// -----------------------------------------

// Mersenne Twister 19937 generator
// A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
// http://www.cplusplus.com/reference/random/mt19937/

std::mt19937 MersenneTwisterPRNG;

// Uniform real distribution
// Random number distribution that produces floating-point values according to a uniform distribution,
// which is described by the following probability density function (...) - more details here:
// http://www.cplusplus.com/reference/random/uniform_real_distribution/

std::uniform_real_distribution<double> m_URD;

// Hemisphere sampling (i.e., for diffuse reflection)
// function "m_Vector HemisphereSampling(m_Vector m_normal)" below
// calls both m_RND_1 and m_RND_2
#define m_RND_1 (2.0*m_URD(MersenneTwisterPRNG)-1.0)    // [-1,1]
#define m_RND_2 (m_URD(MersenneTwisterPRNG))        // [0,1]

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

Point2D Scene::calc_image_coords(Point3D pt)
{
    Matrix4x4 m_view = cam.get_view_matrix();
    Matrix4x4 *m_projection = cam.calc_proj_matrix();

    float width = cam.imgWidth;
    float height = cam.imgHeight;

    Matrix4x4 m_screenCoords = Matrix4x4();           // make transform for converting NDC space to screenspace
    m_screenCoords[0][0] = -width / 2;
    m_screenCoords[1][1] = height / 2;
    m_screenCoords = Matrix4x4::translation(Vector3D(width / 2, height / 2, 0)) * m_screenCoords;

    // Apply the view matrix
    pt = m_view * pt;

    // Do clipping here...
    //bool skipLine = false;
    //Point3D pt_i;

    //do_clip();

    // Apply the projection matrix to 4D points
    Point4D a_4d = Point4D(pt);

    a_4d = (*m_projection) * a_4d;

    // homogenization & store back into 3D points
    pt = Point3D(a_4d[0] / a_4d[3], a_4d[1] / a_4d[3], a_4d[2] / a_4d[3]);

    // map to viewport
    pt = m_screenCoords * pt;

    //delete(m_view);
    delete(m_projection);

    return Point2D(pt[0], pt[1]);
}

Color *Scene::Render(vector<photon*> *photon_map)
{
    Color * result = new Color[cam.imgWidth * cam.imgHeight];
    // iterate over the pixels & set colour values

    for (int x = 0; x < cam.imgWidth; x++)
    {
        for (int y = 0; y < cam.imgHeight; y++)
        {
            result[x + y * cam.imgWidth] = Color(0, 0, 0);  // initialize to all black
        }
    }

    for(std::vector<photon*>::iterator it = photon_map->begin(); it != photon_map->end(); ++it)
    {
        photon *p = (*it);

        // calc image space coordinates
        Point2D img_coords = calc_image_coords(p->get_position());
        int x = (int)img_coords[0];
        int y = (int)img_coords[1];
        int idx = x + y * cam.imgWidth;
        if (idx < 0 || idx >= cam.imgHeight * cam.imgWidth)
            continue;
        Color &pixel = result[x + y * cam.imgWidth];
        pixel = *(p->get_color());
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

    out_clr = *in_clr;
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

    RayType ray_type = russian_roulette(&i_mat);

    if (ray_type == RayType::Diffuse)
        i_clr = Color(i_clr.R() * i_mat.Kd.R(), i_clr.G() * i_mat.Kd.G(), i_clr.B() * i_mat.Kd.B());

    printf("%d: %f %f %f - %f %f %f - %f %f %f\n", depth, (start_pos)[0], (start_pos)[1], (start_pos)[2],
            direction[0], direction[1], direction[2],
            i_clr.R(), i_clr.G(), i_clr.B()
            );

    // store photon in photon list
    photon *store_photon = new photon();
    store_photon->set_position(i_point);
    store_photon->set_direction(direction);
    store_photon->set_color(i_clr);
    out_list->push_back(store_photon);

    bounce_photon(ray_type, &i_point, &i_normal, &i_reflect, &i_refract, &i_clr, depth, out_list);
}

void Scene::emit_photons(int num_photons, vector<photon*> *photon_map)
{
    vector<photon*> photons;

    initialize_photons(num_photons, &photons);    // spawn initial photons for all lights

    for(std::vector<photon*>::iterator it = photons.begin(); it != photons.end(); ++it)
    {
        photon *obj = (*it);
        trace_photon(obj, 0, photon_map);
    }
}

void Scene::bounce_photon(RayType ray_type, Point3D *i_pos, Vector3D *i_normal, Vector3D *i_reflect,
                          Vector3D *i_refract, Color *i_clr, int depth, vector<photon*> *out_list)
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

// phong brdf
// incident direction
// reflected direction
Color BRDF(Point3D x, Vector3D v, Vector3D pd)
{

}

Color radiance_estimate(kdtree *kd, SurfacePoint end_pt)
{
    // how much light is at this point?
    // locate k nearest photons
    // how much light from each

    kdres *kdr = kd_nearest3(kd, end_pt.position[0], end_pt.position[1], end_pt.position[2]);
    photon **photon_set = (photon**)kdr; // array of references?

    Vector3D delta = (*photon_set)[0].get_position() - end_pt.position;
    double r = delta.length();  //distance to kth nearest photon
    Color flux = Color(0,0,0);

    int num_photons = 1;

    for (int i = 0; i < num_photons; i++)
    {
        photon *ph = *photon_set;
        Vector3D pd = ph->get_direction();
        unsigned char *pw = ph->p;

        Color diff;
        Color spec;

        Vector3D norm = end_pt.normal;
        Vector3D light_dir = ph->get_direction(); // incoming light
        Vector3D eye_dir = ph->get_direction(); // eye direction
        Vector3D ref_dir = light_dir + (2 * norm.dot(-light_dir)) * norm;

        diff = light_dir.dot(end_pt.normal) * end_pt.material.Kd;
        spec = pow(ref_dir.dot(eye_dir), end_pt.material.p) * end_pt.material.Ks;

        flux = flux + diff + spec;

        //flux += BRDF(x, v, pd) * pw;

        photon_set++;
    }
/*
    //for (std::vector<photon*>::iterator it = photons->begin(); it != photons->end(); ++it)

    {
        photon *obj = (*it);
        Vector3D pd = obj->get_direction();
        char *pw = obj->p;
        v = dir to eye;
        x = end_pt;
        flux += BRDF(x, v, pd) * pw;
    }*/

    return flux / (2 * M_PI * pow(r,2));
}

Color *Scene::Render(kdtree *kd)
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

            Ray ray = Ray(p, v);

            Color &c = result[x + y * cam.imgWidth];
            SurfacePoint end_pt;

            if (!trace_ray(ray, &end_pt, 1))   // if ray hit nothing
                c = BG_COLOR;                       // use background color
            else
            {
                // gather the photons around the end pt
                c = radiance_estimate(kd, end_pt);
            }
        }
    }
    return result;
}
/*
bool Scene::trace_ray(Ray ray, SurfacePoint *end_pt, Color *color, int depth)
{
    if (depth > MAX_DEPTH)  // stop recursing
        return false;

    double t_min = INFINITY;
    Vector3D n_min;
    SceneObject *hitObject = NULL;
    Vector3D v = ray.direction;
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

    Vector3D n = n_min;
    Vector3D l = light->position - p;
    l.normalize();
    Vector3D r = -l + 2 * (l.dot(n)) * n;

    // add radiance estimate
    //radiance_estimate(kd,);

    Color reflect_rgb;     // reflection color;

    // calculate reflect vector
    Vector3D r = v + (2 * n_min.dot(-v)) * n_min;

    Ray new_ray = Ray(p_int, r);

    trace_ray(new_ray, p_int, r, &reflect_rgb, depth + 1);
    // add reflection color
    col = col + reflect_rgb * hitObject->material->Kr;
    return true;
}
*/

// cornell scene
void Scene::SetupCornellBox(int width, int height)
{
    Scene &scene = *this;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    //cam.lookAt = Point3D(0, 0, -1);
    cam.position = Point3D(0, 0, 0);
    cam.fov = 53.1301024 / 180 * M_PI;
    cam.near = 1;
    cam.far = 10;
    cam.aspect = 1;//(float)width / height;

    scene.cam = cam;
    float img_plane_w = 0.5f;
    cam.m_view = Matrix4x4::translation(Vector3D(0,0,0));
    cam.m_view = cam.m_view.rotation(M_PI, 'y');

    scene.imgPlane = cam.calc_img_plane();
    for (int y = 0; y < 4; y++)
    {
        printf("%f %f %f\n", scene.imgPlane->points[y][0], scene.imgPlane->points[y][1], scene.imgPlane->points[y][2], scene.imgPlane->points[y][3]);
    }

    scene.imgPlane = new Plane(Point3D(-img_plane_w, img_plane_w, -1), Point3D(-img_plane_w, -img_plane_w, -1), Point3D(img_plane_w, -img_plane_w, -1), Point3D(img_plane_w, img_plane_w, -1));
    for (int y = 0; y < 4; y++)
    {
        printf("%f %f %f\n", scene.imgPlane->points[y][0], scene.imgPlane->points[y][1], scene.imgPlane->points[y][2], scene.imgPlane->points[y][3]);
    }

    Material *mat_ceil = new Material(Color(0, 0, 0), Color(1, 1, 1), Color(0, 0, 0), 1000, Color(0, 0, 0));
    Material *mat_grn = new Material(Color(0, 0, 0), Color(0, 0.5f, 0), Color(0, 0, 0), 100, Color(0, 0, 0));
    Material *mat_red = new Material(Color(0, 0, 0), Color(0.5f, 0, 0), Color(0, 0, 0), 10, Color(0, 0, 0));
    Material *mat_floor = new Material(Color(0, 0, 0), Color(0.6f, 0.6f, 0.6f), Color(0, 0, 0), 10, Color(0, 0, 0));

    Light *light = new Light(Point3D(0, 2.65, -8), Color(0.1, 0.1, 0.1), Color(1, 1, 1), Color(0, 0, 0), 1);
    scene.lights.push_back(light);

    // Ceiling
    Quad *q_1 = new Quad(
        Point3D(2.75, 2.75, -10.5),
        Point3D(2.75, 2.75, -5),
        Point3D(-2.75, 2.75, -5),
        Point3D(-2.75, 2.75, -10.5),
        mat_ceil);

    scene.objects.push_back(q_1);

    // Ceiling light
    /*Quad *light_q = new Quad(
        Point3D(0.653, 2.74, -8.274),
        Point3D(-0.653, 2.74, -8.274),
        Point3D(-0.653, 2.74, -7.224),
        Point3D(0.653, 2.74, -7.224),
        mat_red);
    scene.objects.push_back(light_q);
    */

    // Green wall on left
    Quad *q_2 = new Quad(
        Point3D(-2.75, 2.75, -10.5),
        Point3D(-2.75, 2.75, -5),
        Point3D(-2.75, -2.75, -5),
        Point3D(-2.75, -2.75, -10.5),
        mat_grn);
    scene.objects.push_back(q_2);

    //   // Red wall on right
    Quad *q_3 = new Quad(
        Point3D(2.75, 2.75, -10.5),
        Point3D(2.75, -2.75, -10.5),
        Point3D(2.75, -2.75, -5),
        Point3D(2.75, 2.75, -5),
        mat_red);
    scene.objects.push_back(q_3);

    //   // Floor
    Quad *q_4 = new Quad(
        Point3D(2.75, -2.75, -10.5),
        Point3D(-2.75, -2.75, -10.5),
        Point3D(-2.75, -2.75, -5),
        Point3D(2.75, -2.75, 5),
        mat_floor);
    scene.objects.push_back(q_4);

    // Back wall
    Quad *q_5 = new Quad(
        Point3D(2.75, 2.75, -10.5),
        Point3D(-2.75, 2.75, -10.5),
        Point3D(-2.75, -2.75, -10.5),
        Point3D(2.75, -2.75, -10.5),
        mat_floor);
    scene.objects.push_back(q_5);

    Cube *sml_cube = new Cube(
        Point3D(0, 0, 0),
        0.5,
        mat_ceil
    );
    sml_cube->Transform(
        Matrix4x4::translation(Vector3D(-0.929, -2.75 + 3.31 / 2, -8.482)) *
        Matrix4x4::rotation(misc::degToRad(-18.809), 'y') *
        Matrix4x4::scaling(Vector3D(1.659, 3.31, 1.659))
    );
    scene.objects.push_back(sml_cube);

    Cube *big_cube = new Cube(
        Point3D(0,0,0),
        0.5,
        mat_ceil
    );
    big_cube->Transform(
        Matrix4x4::translation(Vector3D(0.933, -2.75 + 1.655 / 2, -6.648)) *
        Matrix4x4::rotation(misc::degToRad(16.303), 'y') *
        Matrix4x4::scaling(Vector3D(1.655, 1.655, 1.655))
    );
    scene.objects.push_back(big_cube);
    //scene.Transform(Matrix4x4::rotation(M_PI, 'y'));
}
