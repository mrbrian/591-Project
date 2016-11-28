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

// -----------------------------------------
//    RANDOM NUMBER GENERATION
// -----------------------------------------

// Mersenne Twister 19937 generator
// A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
// http://www.cplusplus.com/reference/random/mt19937/

std::mt19937 MersenneTwisterPRNG;

// Uniform real distribution
// Random number distribution that produces floating-point values according to a uniform distribution,
// which is described by the following probability density function (...) - more details here:
// http://www.cplusplus.com/reference/random/uniform_real_distribution/

std::uniform_real_distribution<double> m_URD;

// Hemisphere sampling (i.e., for diffuse reflection)
// function "m_Vector HemisphereSampling(m_Vector m_normal)" below
// calls both m_RND_1 and m_RND_2
double RAND_2()
{
    return (m_URD(MersenneTwisterPRNG));    // [-1,1]
}

#define m_RND_1 (2.0*m_URD(MersenneTwisterPRNG)-1.0)    // [-1,1]
#define m_RND_2 (m_URD(MersenneTwisterPRNG))        // [0,1]

#endif // MISC_H
