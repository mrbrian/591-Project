#ifndef CAMERA_H
#define CAMERA_H

#include "algebra.h"

class Camera
{
public:
    int imgWidth;       // image resolution width
    int imgHeight;      // image resolution height
    Point3D position;   // camera position
    Point3D lookAt;     // camera lookat

    float fov;
    double aspect;
    double near;
    double far;

    Matrix4x4 *calc_proj_matrix();
    Matrix4x4 *calc_view_matrix();
};

#endif // CAMERA_H
