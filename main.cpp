#include "lodepng.h"
#include "algebra.h"
#include "scene.h"
#include <vector>
#include <iostream>

#define DEF_WIDTH   200
#define DEF_HEIGHT  200

double clamp(double min, double max, double in)
{
    if (in > max)
        return max;
    if (in < min)
        return min;
    return in;
}

// source: http://www.graphics.cornell.edu/online/box/data.html
void SetupCornellBox(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0, 0, -1);
    cam.position = Point3D(0, 0, 0);
    //cam.fov = 63.439966589479205;

    scene.cam = cam;
    scene.imgPlane = new Plane(Point3D(-0.5,0.5,1), Point3D(-0.5,-0.5,1), Point3D(0.5,-0.5,1), Point3D(0.5,0.5,1));

    Material *mat_ceil = new Material(Colour(0,0,0), Colour(1,0.25,0.1), Colour(1,1,1), 1000,  Colour(0.5,0.5,0.5));
    Material *mat_grn = new Material(Colour(0,0,0), Colour(0.05,0.6,0.95), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *mat_red = new Material(Colour(0,0,0), Colour(0.7,0.95,.13), Colour(1,1,1), 10, 0.5 * Colour(0.7,0.95,.13));

    //Quad *q = new Quad(Point3D(-1,0,1),Point3D(-1,0,-1),Point3D(1,0,-1),Point3D(1,0,1), checker);
    //q->Transform( Matrix4x4::translation(Vector3D(0,-1,0)) * Matrix4x4::scaling(Vector3D(2000,2000,2000)));
    //scene.objects.push_back(q);

    Light *light = new Light(Point3D(0,5,0), Colour(0.1,0.1,0.1), Colour(0.5,0.5,0.5), Colour(0,0,0));
    scene.lights.push_back(light);
/*    Light *light2 = new Light(Point3D(10,15,1), Colour(0.1,0.1,0.1), Colour(0.6,0.6,0.5), Colour(1,1,1));
    scene.lights.push_back(light2);
    Light *light3 = new Light(Point3D(-20,20,40), Colour(0.1,0.1,0.1), Colour(0.25,0.25,0.35), Colour(1,1,1));
    scene.lights.push_back(light3);*/

    scene.Transform(
                Matrix4x4::translation(Vector3D(cam.lookAt)) *
                Matrix4x4::rotation(30*M_PI/180,'y') *
                Matrix4x4::rotation(-35*M_PI/180,'x') *
                Matrix4x4::translation(-Vector3D(cam.lookAt)));

    // Ceiling
    Triangle *tri_1 = new Triangle(
      Point3D(2.75, 2.75, -10.5),
      Point3D(2.75, 2.75, -5),
      Point3D(-2.75, 2.75, -5),
      Point3D(1, 1, 1), *mat_ceil);

    Triangle *tri_2 = new Triangle(
      Point3D(-2.75, 2.75, -10.5),
      Point3D(2.75, 2.75, -10.5),
      Point3D(-2.75, 2.75, -5),
      Point3D(1, 1, 1), mat_ceil);

    // Green wall on right
    Triangle *tri_3 = new Triangle(
      Point3D(2.75, 2.75, -5),
      Point3D(2.75, 2.75, -10.5),
      Point3D(2.75, -2.75, -10.5),
      Point3D(0.0, 0.5, 0.0), );


     Triangle *tri_4 = new Triangle(
      Point3D(2.75, -2.75, -5),
      Point3D(2.75, 2.75, -5),
      Point3D(2.75, -2.75, -10.5),
      Point3D(0.0, 0.5, 0.0));


    // Red wall on left
     Triangle *tri_5 = new Triangle(
      Point3D(-2.75, -2.75, -5),
      Point3D(-2.75, -2.75, -10.5),
      Point3D(-2.75, 2.75, -10.5),
      Point3D(0.5, 0.0, 0.0));

     Triangle *tri_6 = new Triangle(
      Point3D(-2.75, 2.75, -5),
      Point3D(-2.75, -2.75, -5),
      Point3D(-2.75, 2.75, -10.5),
      Point3D(0.5, 0.0, 0.0));


    // Floor
     Triangle *tri_6 = new Triangle(
      Point3D(2.75, -2.75, -5),
      Point3D(2.75, -2.75, -10.5),
      Point3D(-2.75, -2.75, -10.5),
      Point3D(0.6, 0.6, 0.6));

     Triangle *tri_6 = new Triangle(
      Point3D(-2.75, -2.75, -5),
      Point3D(2.75, -2.75, -5),
      Point3D(-2.75, -2.75, -10.5),
      Point3D(0.6, 0.6, 0.6));

    // Back wall
    Quad *q = new Quad(
        Point3D(0, 0, 1),
        Point3D(0, 0, -10.5),
        Point3D(0.6, 0.6, 0.6),
        Point3D(0.6, 0.6, 0.6));

    scene.objects.push_back(tri_1);
    scene.objects.push_back(tri_2);
    scene.objects.push_back(tri_3);
    scene.objects.push_back(tri_4);
    scene.objects.push_back(tri_5);
    scene.objects.push_back(tri_6);
    scene.objects.push_back(q);

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

    SetupScene1(&scene, width, height);

    // create new image
	const char *filename = "out.png";

	std::vector<unsigned char> image;
	image.resize(width * height * 4);

    Colour *resultImg = scene.Render();

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int idx = x + y * scene.cam.imgWidth;
			Colour &c = resultImg[idx];
			// clamp rgb values [0,1]
			image[4 * idx + 0] = clamp(0, 1, c.R()) * 255;
			image[4 * idx + 1] = clamp(0, 1, c.G()) * 255;
			image[4 * idx + 2] = clamp(0, 1, c.B()) * 255;
			image[4 * idx + 3] = 255;
		}
	}
    // save to file
	unsigned int error = lodepng::encode(filename, image, width, height);

	/*if there's an error, display it*/
	if (error) 
		printf("error %u: %s\n", error, lodepng_error_text(error));

    // application successfully returned
    return 0;
}
