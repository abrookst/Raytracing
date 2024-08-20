#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vector3.h"

class Color : public Vector3 {
    public:
        Color(float vX, float vY, float vZ): Vector3(vX, vY, vZ) {}
        Color(): Vector3(0,0,0) {}
};

void write_color(std::ostream& out, const Color& pixel_color) {
    float r = pixel_color.x();
    float g = pixel_color.y();
    float b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    uint8_t rbyte = int(255 * r);
    uint8_t gbyte = int(255 * g);
    uint8_t bbyte = int(255 * b);

    // Write out the pixel color components.
    out << +rbyte << ' ' << +gbyte << ' ' << +bbyte << '\n';
}


#endif