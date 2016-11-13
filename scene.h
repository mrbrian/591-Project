#ifndef SCENE
#define SCENE

#include "shapes.h"
#include "algebra.h"
#include "misc.h"
#include "photon.h"
#include <vector>

using namespace std;

class Camera
{
public:
    int imgWidth;       // image resolution width
    int imgHeight;      // image resolution height
    Point3D position;   // camera position
    Point3D lookAt;     // camera lookat
};

class Light
{
public:
    Light(Point3D pos, Colour a, Colour d, Colour s);
    Colour Ia;      // ambient
    Colour Id;      // diffuse
    Colour Is;      // specular
    Point3D position;
    double emission;    // amount of samples

    void Transform(Matrix4x4 m)
    {
        position = m * position;
    }        

    void emit_photons(vector<photon*> *out_photons)
    {
        int num_emit = 0;
        while (num_emit < emission)
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
            p->set_direction(dir);

            out_photons->push_back(p);
            num_emit++;
        }
    }
};


class LightObject
{
public:
    Light *light;
    SceneObject *obj;

    LightObject(Light *light, SceneObject *obj);
};

class Plane
{
public:
    Point3D *points;

    Plane(Point3D a, Point3D b, Point3D c, Point3D d)
    {
        points = new Point3D[4] {a,b,c,d};
    }

    void Transform(Matrix4x4 m)
    {
        for (int i = 0; i < 4 ;i ++)
        {
            points[i] = m * points[i];
        }
    }

    Point3D ImageToWorldSpace(int x, int y, int imgWidth, int imgHeight)
    {
        Point3D topleft  = points[0];
        Point3D topright = points[3];
        Point3D botleft  = points[1];

        Vector3D right = topright - topleft;
        Vector3D down  = botleft - topleft;

        double h_pct = (float)x / imgWidth;
        double v_pct = (float)y / imgHeight;

        Point3D result = (topleft + (h_pct * right) + (v_pct * down));
        return result;
    }
};

class ImagePlane
{
public:
    Point3D center;
    int width;
    int height;

    ImagePlane(Point3D c, int w, int h)
    {
        center = c;
        width = w;
        height = h;
    }
};

class Scene
{
public:
    Camera cam;
    Plane *imgPlane;
    std::vector<SceneObject*> objects;
    std::vector<Light*> lights;
    Colour * Render();

    void Transform(Matrix4x4 m)
    {
        for(std::vector<SceneObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
        {
            SceneObject *obj = (*it);
            obj->Transform(m);
        }
        for(std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
        {
            Light *obj = (*it);
            obj->Transform(m);
        }
    }

    void emit_photons()
    {
        vector<photon*> photons;
        vector<photon*> out_photons;

        spawn_photons(&photons);    // spawn initial photons for all lights

        for(std::vector<photon*>::iterator it = photons.begin(); it != photons.end(); ++it)
        {
            photon *obj = (*it);
            trace_photon(obj, list);
        }
    }

    void spawn_photons(vector<photon*> *out_photons)
    {
        for(std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
        {
            Light *obj = (*it);
            obj->emit_photons(*out_photons);
        }
    }

    // collide photon with the scene objects
    void *trace_photon(photon *in_pho, vector<photon*> out_list)
    {
        // subtract energy according to how far the photon travels before intersection
        //get intersection_pt.
        //color from material
        // absorption...
        // material type (reflect? or diffuse or refract)
        Vector3D start_pos = Vector3D(in_pho->x, in_pho->y, in_pho->z);
        Vector3D direction = Vector3D(getDirection(in_pho->phi, in_pho->theta));

        Material *mat;
        Point3D *intersection_pt;

        if (!trace_primary_ray(start_pos, direction, scene, intersection_pt, mat))
            return NULL;

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
    }
};

bool trace_ray(Point3D o, Vector3D v, Scene *scene, Colour *color, int depth);
bool trace_primary_ray(Scene *scene, Point3D in_pos, Vector3D in_dir, Colour *in_clr, Point3D *out_pos, Vector3D *out_dir, Colour *out_clr, int depth);

#endif //SCENE
