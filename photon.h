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

    photon(){}

    photon(Point3D p, Vector3D d, Color c){
        set_position(p);
        set_direction(d);
        set_color(c);
    }

    void set_position(Point3D p);
    void set_direction(Vector3D dir);
    void set_color(Color dir);
    Color *get_color();
    Point3D get_position();
    Vector3D get_direction();

    float dist2(const photon &n)const{
       float _x = n.x - x;
       float _y = n.y - y;
       float _z = n.z - z;
       return _x*_x + _y*_y + _z*_z;
    }
};

#endif //PHOTON

