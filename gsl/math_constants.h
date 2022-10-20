#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

#include "gltypes.h"

namespace gsl
{
    // Mathematical constants
    constexpr double PI_D = 3.141592653589793238462643383279502884;
    constexpr float PI = 3.14159265358979323846264338f;
    constexpr float E = 2.71828182845904523536f;
    constexpr float GRAVITY = 9.80665f; // Differs depending on where you are on the earth, ~9.81

    ///Converts radians to degrees
    inline GLdouble rad2deg(const GLdouble rad)
    { return rad * (180.0 / PI_D); }

    ///Converts degrees to radians
    inline GLdouble deg2rad(const GLdouble deg)
    { return deg * (PI_D / 180.0); }

    ///Converts radians to degrees
    inline GLfloat rad2degf(const GLfloat rad)
    { return rad * (180.0f / PI); }

    ///Converts degrees to radians
    inline GLfloat deg2radf(const GLfloat deg)
    { return deg * (PI / 180.0f); }
}

#endif // MATH_CONSTANTS_H
