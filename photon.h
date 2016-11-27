#ifndef PHOTON_H
#define PHOTON_H

#define _USE_MATH_DEFINES

#include "algebra.h"

using namespace std;

struct photon {
    float x, y, z;              // position
    unsigned char p[4];         // power packed as 4 chars
    unsigned char phi, theta;   // compressed incident direction
    Vector3D normal;            // surface normal
    short flag;                 // flag used in kdtree

    void set_position(Point3D p);
    void set_direction(Vector3D dir);
    void set_color(Color dir);
    Color *get_color();
    Point3D get_position();
    Vector3D get_direction();
};

#endif //PHOTON

