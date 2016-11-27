#ifndef MISC_H
#define MISC_H

#include <random>

class misc
{
public:
    static double degToRad(float deg)
    {
        return (M_PI / 180 * deg);
    }
};
#endif // MISC_H
