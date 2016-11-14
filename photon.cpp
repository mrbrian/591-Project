#include "photon.h"

void photon::set_position(Point3D p)
{
    x = p[0];
    y = p[1];
    z = p[2];
}

void photon::set_direction(Vector3D dir)
{
    if (dir.length2() > 1.1)
        throw ("Normalize plz");
    if (dir.length2() < 0.9)
        throw ("Normalize plz");
    double t = atan2(dir[2], dir[0]);
    t += M_PI;
    t /= (2 * M_PI);
    double t2 = acos(dir[1]);
    t2 /= M_PI;

    phi = (char)((255 * (atan2(dir[2], dir[0]) + M_PI)) / (2 * M_PI));
    theta = (char)(255 * acos(dir[1]) / M_PI);
}

void photon::set_color(Color dir)
{
    p[0] = (char)(dir.R() * 255);
    p[1] = (char)(dir.R() * 255);
    p[2] = (char)(dir.R() * 255);
    p[3] = 0;//(char)(dir.R() * 255);
}

Color *photon::get_color()
{
    return new Color(
            (double)p[0] / 255,
            (double)p[1] / 255,
            (double)p[2] / 255);
}

Point3D photon::get_position()
{
    return Point3D(x, y, z);
}

Vector3D photon::get_direction()
{
    double ph = ((float)phi * 2 * M_PI) / 255.0 - M_PI;
    double th = (float)theta / 255.0 * M_PI;

    float x = cos(ph) * sin(th);
    float z = sin(ph) * sin(th);
    float y = cos(th);

    return Vector3D(x, y, z);
}
