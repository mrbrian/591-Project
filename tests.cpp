#include "tests.h"
#include "scene.h"
#include <stdio.h>
#include <math.h>
#include "lodepng.h"

using namespace std;

double clamp(double min, double max, double in)
{
    if (in > max)
        return max;
    if (in < min)
        return min;
    return in;
}

void sphericalCoord_1()
{
    photon p;

    vector<Vector3D> list = {Vector3D(1,1,1), Vector3D(-1,-1,-1),
                             Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1), Vector3D(-1, 0, 0), Vector3D(0, -1, 0), Vector3D(0, 0, -1)};

    for(std::vector<Vector3D>::iterator it = list.begin(); it != list.end(); ++it)
    {
        Vector3D expected = (*it);
        expected.normalize();
        p.set_direction(expected);
        printf("%d, %d\n", p.phi, p.theta);
        Vector3D actual = p.get_direction();
        printf("%f, %f, %f\n", expected[0], expected[1], expected[2]);
        printf("%f, %f, %f\n", actual[0], actual[1], actual[2]);
        if ((expected - actual).length2() > 0.5)
            throw 1;
    }
}

#define DEF_VIEW_NEAR       1
#define DEF_VIEW_FAR        10
#define DEF_VIEW_FOV        90
#define DEF_VIEW_DIST       1

void proj_point()
{
    printf("proj_point\n");
    vector<photon*> p_list;
    {
        photon *p = new photon();
        p->set_position(Point3D(1,0,1));
        p->set_direction(Vector3D(0,0,1));
        p->set_color(Color(1,0,1));
        p_list.push_back(p);
    }
    photon *p2 = new photon();
    p2->set_position(Point3D(0,1,1));
    p2->set_direction(Vector3D(0,0,1));
    p2->set_color(Color(0,1,1));
    p_list.push_back(p2);

    photon *p3 = new photon();
    p3->set_position(Point3D(1,1,1));
    p3->set_direction(Vector3D(0,0,1));
    p3->set_color(Color(0,1,0));
    p_list.push_back(p3);

    {
        photon *p = new photon();
        p->set_position(Point3D(-1,-1,1));
        p->set_direction(Vector3D(1,0,0));
        p->set_color(Color(1,1,0));
        p_list.push_back(p);
    }

    Scene s;
    s.cam.position = Point3D(0,0,0);

    //float aspect = (float)width() / height();

    s.cam.fov = (float)DEF_VIEW_FOV / 180 * M_PI;
    s.cam.far = DEF_VIEW_FAR;
    s.cam.near = DEF_VIEW_NEAR;
    s.cam.aspect = 1;
    s.cam.imgHeight = 5;
    s.cam.imgWidth= 5;

    Matrix4x4 m;
    m.translation(Vector3D(0,0,0));
    s.cam.m_view = m;

    float img_plane_w = 0.5f;
    s.imgPlane = s.cam.calc_img_plane();
    for (int y = 0; y < 4; y++)
    {
        printf("%f %f %f\n", s.imgPlane->points[y][0], s.imgPlane->points[y][1], s.imgPlane->points[y][2], s.imgPlane->points[y][3]);
    }

    int width = s.cam.imgWidth;
    int height = s.cam.imgHeight;

    std::vector<unsigned char> img;
    img.resize(width * height * 4);

    Color *image = s.Render(&p_list);
    for (int y = 0; y < s.cam.imgHeight; y++)
    {
        for (int x = 0; x < s.cam.imgWidth; x++)
        {
            printf("%d,%d - %f %f %f\n", x,y,image[y*s.cam.imgWidth + x].R(), image[y*s.cam.imgWidth + x].G(), image[y*s.cam.imgWidth + x].B());
        }
    }

    for (int x = 0; x < s.cam.imgWidth; x++)
    {
        for (int y = 0; y < s.cam.imgHeight; y++)
        {
            int idx = x + y * s.cam.imgWidth;
            Color &c = image[idx];
            // clamp rgb values [0,1]
            img[4 * idx + 0] = clamp(0, 1, c.R()) * 255;
            img[4 * idx + 1] = clamp(0, 1, c.G()) * 255;
            img[4 * idx + 2] = clamp(0, 1, c.B()) * 255;
            img[4 * idx + 3] = 255;
        }
    }
    // save to file
    lodepng::encode("proj_point", img, s.cam.imgWidth, s.cam.imgHeight);

}

void proj_point2()
{
    printf("proj_point2\n");
    Scene s;
    s.cam.position = Point3D(0,0,0);

    //float aspect = (float)width() / height();

    s.cam.fov = (float)DEF_VIEW_FOV / 180 * M_PI;
    s.cam.far = DEF_VIEW_FAR;
    s.cam.near = DEF_VIEW_NEAR;
    s.cam.aspect = 1;
    s.cam.imgHeight = 5;
    s.cam.imgWidth= 5;

    Matrix4x4 m;
    m = m.translation(Vector3D(0,0,0));
    m = m.rotation(M_PI, 'y');
    s.cam.m_view = m;

    float img_plane_w = 0.5f;
    s.imgPlane = s.cam.calc_img_plane();
    for (int y = 0; y < 4; y++)
    {
        printf("%f %f %f\n", s.imgPlane->points[y][0], s.imgPlane->points[y][1], s.imgPlane->points[y][2], s.imgPlane->points[y][3]);
    }
}

tests::tests()
{
    sphericalCoord_1();
    proj_point();
    proj_point2();
}

int main(int argc, char *argv[])
{
    tests t;

    return 0;
}
