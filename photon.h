#include "scene.h"
#include <algorithm>

struct photon {
	float x, y, z;    // position
	char p[4];        // power packed as 4 chars
	char phi, theta;  // compressed incident direction
	short flag;       // flag used in kdtree
};

class Photons
{
/*	enum RayType
	{
		Diffuse,
		Specular,
		Absorb
	};

	void *Shoot(Scene *scene, Light *light, Point3D position, Vector3D direction)
	{
		// shoot a primary ray 
		// get intersect point and material
		// save into photon map
		// roll the dice
		// recurse

		Colour *c = new Colour();
		if (trace_ray(position, position, scene, c, 1, 1))
		{
			new photon (intersect_pt, colour)
				//store new photon
		}
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
		return RayType::Absorb;
	}*/
};