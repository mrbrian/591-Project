#include "tests.h"
#include "scene.h"
#include <stdio.h>

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

void proj_point()
{
    vector<photon*> p;

    vector<Vector3D> list = {Vector3D(1,1,1), Vector3D(-1,-1,-1),
                             Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1), Vector3D(-1, 0, 0), Vector3D(0, -1, 0), Vector3D(0, 0, -1)};

    Scene s;
    s.Render(p);

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

tests::tests()
{
    sphericalCoord_1();
    proj_point();
}

int main(int argc, char *argv[])
{
    tests t;

    return 0;
}
