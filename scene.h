#ifndef SCENE
#define SCENE

#include "shapes.h"
#include "algebra.h"
#include "photon.h"
#include <vector>
#include "polyroots.h"
#include "camera.h"

using namespace std;

class Light
{
public:
    Light(Point3D pos, Color a, Color d, Color s, double in_watts);
    Color Ia;      // ambient
    Color Id;      // diffuse
    Color Is;      // specular
    Point3D position;
    double watts;

    void emit_photons(int to_emit, vector<photon*> *out_photons);

    void Transform(Matrix4x4 m)
    {
        position = m * position;
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
    Color * Render();
    Color *Render(vector<photon*> *photon_map);

    bool trace_ray(Point3D o, Vector3D v, Color *color, int depth);
    bool trace_ray(Point3D o, Vector3D v, Color *color, Point3D *out_pos, Vector3D *out_norm, Color *out_clr, Material *out_mat, int depth);
    bool trace_primary_ray(Point3D in_pos, Vector3D in_dir, Color *in_clr, Point3D *_out_pos, Vector3D *_out_norm, Vector3D *_out_reflect, Vector3D *_out_refract, Color *_out_clr, Material *_out_mat);   
    Point2D calc_image_coords(Point3D pt);

    // collide photon with the scene objects
    void trace_photon(photon *in_pho, int depth, vector<photon*> *out_list);
    void emit_photons(int num_photons, vector<photon*> *photon_map);
    void bounce_photon(RayType ray_type, Point3D *i_pos, Vector3D *i_norm, Vector3D *i_reflect, Vector3D *i_refract, Color *i_clr, int depth, vector<photon*> *out_list);
    RayType russian_roulette(Material *mat);
    void initialize_photons(int num_photons, vector<photon*> *out_photons);

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
