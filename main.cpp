#include "lodepng.h"
#include "algebra.h"
#include "scene.h"
#include <vector>
#include <iostream>

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

double degToRad(float deg)
{
	return (M_PI / 180 * deg);
}

// cornell scene
void SetupCornellBox(Scene *s, int width, int height)
{
	Scene &scene = *s;
	Camera cam = Camera();

	cam.imgWidth = width;
	cam.imgHeight = height;
	cam.lookAt = Point3D(0, 0, -1);
	cam.position = Point3D(0, 0, 0);

	scene.cam = cam;
	float img_plane_w = 0.5f;
	scene.imgPlane = new Plane(Point3D(-img_plane_w, img_plane_w, -1), Point3D(-img_plane_w, -img_plane_w, -1), Point3D(img_plane_w, -img_plane_w, -1), Point3D(img_plane_w, img_plane_w, -1));

	Material *mat_ceil = new Material(Colour(0, 0, 0), Colour(1, 1, 1), Colour(1, 1, 1), 1000, Colour(0, 0, 0));
	Material *mat_grn = new Material(Colour(0, 0, 0), Colour(0, 0.5f, 0), Colour(1, 1, 1), 100, Colour(0, 0, 0));
	Material *mat_red = new Material(Colour(0, 0, 0), Colour(0.5f, 0, 0), Colour(1, 1, 1), 10, Colour(0, 0, 0));
	Material *mat_floor = new Material(Colour(0, 0, 0), Colour(0.6f, 0.6f, 0.6f), Colour(1, 1, 1), 10, Colour(0, 0, 0));

	Light *light = new Light(Point3D(0, 2.65, -8), Colour(0.1, 0.1, 0.1), Colour(0.5, 0.5, 0.5), Colour(0, 0, 0));
	scene.lights.push_back(light);

	// Ceiling
	Quad *q_1 = new Quad(
		Point3D(2.75, 2.75, -10.5),
		Point3D(2.75, 2.75, -5),
		Point3D(-2.75, 2.75, -5),
		Point3D(-2.75, 2.75, -10.5),
		mat_ceil);

	scene.objects.push_back(q_1);

	// Ceiling light
	Quad *light_q = new Quad(
		Point3D(0.653, 2.74, -8.274),
		Point3D(-0.653, 2.74, -8.274),
		Point3D(-0.653, 2.74, -7.224),
		Point3D(0.653, 2.74, -7.224),
		mat_red);
	scene.objects.push_back(light_q);

	// Green wall on right
	Quad *q_2 = new Quad(
		Point3D(-2.75, 2.75, -10.5),
		Point3D(-2.75, 2.75, -5),
		Point3D(-2.75, -2.75, -5),
		Point3D(-2.75, -2.75, -10.5),
		mat_grn);
	scene.objects.push_back(q_2);

	//   // Red wall on left
	Quad *q_3 = new Quad(
		Point3D(2.75, 2.75, -10.5),
		Point3D(2.75, -2.75, -10.5),
		Point3D(2.75, -2.75, -5),
		Point3D(2.75, 2.75, -5),
		mat_red);
	scene.objects.push_back(q_3);

	//   // Floor
	Quad *q_4 = new Quad(
		Point3D(2.75, -2.75, -10.5),
		Point3D(-2.75, -2.75, -10.5),
		Point3D(-2.75, -2.75, -5),
		Point3D(2.75, -2.75, 5),
		mat_floor);
	scene.objects.push_back(q_4);

	// Back wall
	Quad *q_5 = new Quad(
		Point3D(2.75, 2.75, -10.5),
		Point3D(-2.75, 2.75, -10.5),
		Point3D(-2.75, -2.75, -10.5),
		Point3D(2.75, -2.75, -10.5),
		mat_floor);
	scene.objects.push_back(q_5);

	Cube *sml_cube = new Cube(
		Point3D(0, 0, 0),
		0.5,
		mat_ceil
	);
	sml_cube->Transform(
		Matrix4x4::translation(Vector3D(-0.929, -2.75 + 3.31 / 2, -8.482)) *
		Matrix4x4::rotation(degToRad(-18.809), 'y') *
		Matrix4x4::scaling(Vector3D(1.659, 3.31, 1.659))
	);
	scene.objects.push_back(sml_cube);

	Cube *big_cube = new Cube(
		Point3D(0,0,0),
		0.5,
		mat_ceil
	);
	big_cube->Transform(
		Matrix4x4::translation(Vector3D(0.933, -2.75 + 1.655 / 2, -6.648)) *
		Matrix4x4::rotation(degToRad(16.303), 'y') *
		Matrix4x4::scaling(Vector3D(1.655, 1.655, 1.655)) 
	);
	scene.objects.push_back(big_cube);
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

    SetupCornellBox(&scene, width, height);

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
