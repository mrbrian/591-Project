#include <stdio.h>
#include <assert.h>
#include "lodepng.h"
#include "algebra.h"
#include "scene.h"
#include <vector>
#include <iostream>
#include "kdtree/kdtree.h"
#include "misc.h"

#define DEF_WIDTH   250
#define DEF_HEIGHT  250

double clamp(double min, double max, double in)
{
    if (in > max)
        return max;
    if (in < min)
        return min;
    return in;
}

void* make_kdtree(vector<photon*> *photon_map)
{
    void *kd;
    kd = kd_create(3);

    for (vector<photon*>::iterator it = (*photon_map).begin(); it != (*photon_map).end(); ++it)
    {
        photon *obj = (*it);

        //kd_insert3((kdtree*)kd, obj->x, obj->y, obj->z, 0);
        assert(kd_insert3((kdtree*)kd, obj->x, obj->y, obj->z, 0) == 0);
    }

}

void render_photons(Scene scene, vector<photon*> *photon_map, const char* outputStr)
{
    int width = scene.cam.imgWidth;
    int height = scene.cam.imgHeight;

    Color *resultImg = scene.Render(photon_map);

    misc::save_color_image(outputStr, resultImg, width, height);
    delete (resultImg);
}


void normal_render(Scene scene, const char* outputStr)
{
    int width = scene.cam.imgWidth;
    int height = scene.cam.imgHeight;

    // create new image
    std::vector<unsigned char> image;
    image.resize(width * height * 4);

    Color *resultImg = scene.Render();

    misc::save_color_image(outputStr, resultImg, width, height);
    delete (resultImg);
}

void final_render(Scene scene, vector<photon*> *photons, const char* outputStr)
{
    int width = scene.cam.imgWidth;
    int height = scene.cam.imgHeight;

    void *tree = make_kdtree(photons);

    // create new image
    std::vector<unsigned char> image;
    image.resize(width * height * 4);

    Color *resultImg = scene.Render((kdtree*)tree);

    misc::save_color_image(outputStr, resultImg, width, height);
    delete (resultImg);
}

int main(int argc, char *argv[])
{
    Scene scene;

    // image width and height
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    char const *outputStr = "output.png";
    if (argc >= 3)                  // prompt user on command line for dimensions
    {
        width  = atoi(argv[1]);
        height = atoi(argv[2]);
        outputStr = argv[3];
    }

    scene = *Scene::cornellBoxScene(width, height);

    vector<photon*> photon_map;
    scene.emit_photons(100000, &photon_map);

    normal_render(scene, "standard");
    render_photons(scene, &photon_map, "photons");
    final_render(scene, &photon_map, "final");

    // application successfully returned
    return 0;
}
