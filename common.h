#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}

inline float random_float() {
    return std::rand() / (RAND_MAX + 1.0f);
}

inline float random_float(float min, float max) {
    return min + (max-min)*random_float();
}

// Common Headers
#include "vector3.h"
#include "interval.h"
#include "color.h"
#include "ray.h"

#endif