#include "photon.h"

void photon::set_position(Point3D p)
{
    x = p[0];
    y = p[1];
    z = p[2];
}

void photon::set_direction(Vector3D dir)
{
    phi = 255 * (atan2(dir.get_y(), dir.get_x()) + M_PI/ (2 * M_PI));
    theta = 255 * acos(dir.get_x()) / M_PI;
}

void photon::set_color(Color dir)
{
    p[0] = (char)(dir.R() * 255);
    p[1] = (char)(dir.R() * 255);
    p[2] = (char)(dir.R() * 255);
    p[3] = 0;//(char)(dir.R() * 255);
}

Point3D photon::get_position()
{
    return Point3D(x, y, z);
}

Vector3D photon::get_direction()
{
    return Vector3D(-1,-1,-1);
}
