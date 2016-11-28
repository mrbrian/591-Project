#ifndef MISC_H
#define MISC_H

#include <random>
#include "lodepng/lodepng.h"
#include "algebra.h"

class misc
{
public:
    static double clamp(double min, double max, double in)
    {
        if (in > max)
            return max;
        if (in < min)
            return min;
        return in;
    }

    static double degToRad(float deg)
    {
        return (M_PI / 180 * deg);
    }

    static void save_color_image (const char *filename, Color *image, int width, int height)
    {
        std::vector<unsigned char> img;
        img.resize(width * height * 4);

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                int idx = x + y * width;
                Color &c = image[idx];
                // clamp rgb values [0,1]
                img[4 * idx + 0] = clamp(0, 1, c.R()) * 255;
                img[4 * idx + 1] = clamp(0, 1, c.G()) * 255;
                img[4 * idx + 2] = clamp(0, 1, c.B()) * 255;
                img[4 * idx + 3] = 255;
            }
        }

        // save to file
        lodepng::encode(filename, img, width, height);
    }
};
#endif // MISC_H
