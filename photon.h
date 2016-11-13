#ifndef PHOTON_H
#define PHOTON_H

#define _USE_MATH_DEFINES

#include "algebra.h"

using namespace std;

struct photon {
	float x, y, z;    // position
    char p[4];        // power packed as 4 chars
    char phi, theta;  // compressed incident direction
	short flag;       // flag used in kdtree

    void set_position(Point3D p);
    void set_direction(Vector3D dir);
    void set_color(Color dir);
    Point3D get_position();
    Vector3D get_direction();
};
/*
class Photons
{
    enum RayType
	{
		Diffuse,
		Specular,
        Transmission
	};

    void Shoot(Scene *scene,
                _Photon *pho_in,
                vector<_Photon*> *photon_map)
    {
		// shoot a primary ray 
		// get intersect point and material
		// save into photon map
		// roll the dice
        // recurse
        _Photon *pho_out = update_photon(scene, pho_in);

        if (pho_out != NULL)
        {
            Shoot(scene, pho_out, photon_map);
        }
	}

    // collide photon with the scene objects
    _Photon *trace_photon(Scene *scene, _Photon *pho_in)
    {
        // subtract energy according to how far the photon travels before intersection
        //get intersection_pt.
        //color from material
        // absorption...
        // material type (reflect? or diffuse or refract)
        Vector3D start_pos = Vector3D(pho_in->x, pho_in->y, pho_in->z);
        Vector3D direction = Vector3D(getDirection(pho_in->phi, pho_in->theta));

        Material *mat;
        Point3D *intersection_pt;

        if (!trace_primary_ray(start_pos, direction, scene, intersection_pt, mat))
            return NULL;

        // subtract energy depending on absorption
        pho_in->p[3] -= 1;

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
    }

	RayType roulette(Vector3D diff, Vector3D spec)   // [0, d] diffuse reflection ray, [d,s+d] specular ray, [s+d, 1] absorption
	{
		double p_refl = std::max(diff.get_x() + spec.get_x(), diff.get_y() + spec.get_y());
		p_refl = std::max(p_refl, diff.get_z() + spec.get_z());

		float p_diff = diff.coord_sum() / (diff.coord_sum() + spec.coord_sum()) * p_refl;
		float p_spec = spec.coord_sum() / (diff.coord_sum() + spec.coord_sum()) * p_refl;
		
		float r = std::rand() % 1000;

		if (r >= 0 && r < p_diff) 
			return RayType::Diffuse;
		if (r >= p_diff && r < p_spec) 
			return RayType::Specular;
        return RayType::Transmission;
    }
};

*/
#endif //PHOTON

