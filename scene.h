#ifndef SCENE
#define SCENE

#include "shapes.h"
#include "algebra.h"
#include "photon.h"
#include <vector>
#include "polyroots.h"
#include "camera.h"
#include "kdtree/kdtree.h"
#include "misc.h"

using namespace std;

struct SurfacePoint
{
public:
    Point3D position;
    Vector3D normal;
    Material *material;

    SurfacePoint() {}

    SurfacePoint(Point3D p, Vector3D v, Material *m)
    {
        position = p;
        normal = v;
        material = m;
    }
};

struct Ray
{
public:
    Point3D origin;
    Vector3D direction;

    Ray(Point3D p, Vector3D v)
    {
        origin = p;
        direction = v;
    }
};

class Light
{
public:
    Color Ia;      // ambient
    Color Id;      // diffuse
    Color Is;      // specular
    Point3D position;
    double watts;

    Light(Point3D pos, Color a, Color d, Color s, double in_watts);
    virtual void emit_photons(int to_emit, vector<photon*> *out_photons);

    virtual void Transform(Matrix4x4 m)
    {
        position = m * position;
    }
};

class LightObject : public Light
{
public:
    SceneObject *obj;

    LightObject(Point3D pos, Color a, Color d, Color s, double in_watts, SceneObject *obj);
    void emit_photons(int to_emit, vector<photon*> *out_photons) override;
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
    enum RayType
    {
        Diffuse,
        Specular,
        Transmission
    };

    Camera cam;
    Plane *imgPlane;
    std::vector<SceneObject*> objects;
    std::vector<Light*> lights;

    static Scene *cornellBoxScene(int width, int height);

    Color *Render();
    Color *Render(vector<photon*> *photon_map);
    Color *Render(kdtree *kd);
    Color Render(kdtree *kd, int x, int y);

    bool trace_ray(kdtree *kd, Ray ray, Color *color, int depth);
    bool trace_ray(Point3D o, Vector3D v, Color *color, int depth);
    bool trace_ray(Point3D o, Vector3D v, Color *color, Point3D *out_pos, Vector3D *out_norm, Color *out_clr, Material *out_mat, int depth);
    bool trace_primary_ray(Point3D in_pos, Vector3D in_dir, Color *in_clr, Point3D *_out_pos, Vector3D *_out_norm, Vector3D *_out_reflect, Vector3D *_out_refract, Color *_out_clr, Material *_out_mat);   
    Point2D calc_image_coords(Point3D pt);

    // collide photon with the scene objects
    Color BRDF(SurfacePoint x, Vector3D view, Vector3D pd);
    void initialize_photons(int num_photons, vector<photon*> *out_photons);
    RayType russian_roulette(Material *mat);
    void emit_photons(int num_photons, vector<photon*> *photon_map);
    void trace_photon(photon *in_pho, int depth, vector<photon*> *out_list);
    void bounce_photon(RayType ray_type, Point3D *i_pos, Vector3D *i_norm, Vector3D *i_reflect, Vector3D *i_refract, Color *i_clr, int depth, vector<photon*> *out_list);
    Color radiance_estimate(kdtree *kd, SurfacePoint end_pt);

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
};

#endif //SCENE
