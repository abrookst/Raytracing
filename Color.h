#ifndef COLOR_H
#define COLOR_H

#include "common.h"

class Color : public Vector3 {
    public:
        Color(float vX, float vY, float vZ): Vector3(vX, vY, vZ) {}
        Color(): Vector3(0,0,0) {}
        Color(Vector3 vec): Vector3(vec) {}
};

inline void linear_to_gamma(float& linearComponent)
{
    if (linearComponent > 0){
        linearComponent = std::sqrt(linearComponent);
        return;
    }
    linearComponent = 0;
}

void write_color(std::ostream& out, const Color& pixel_color) {
    float r = pixel_color.x();
    float g = pixel_color.y();
    float b = pixel_color.z();

    linear_to_gamma(r);
    linear_to_gamma(g);
    linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval intensity(0.000, 0.999);
    uint8_t rbyte = int(255 * intensity.clamp(r));
    uint8_t gbyte = int(255 * intensity.clamp(g));
    uint8_t bbyte = int(255 * intensity.clamp(b));

    // Write out the pixel color components.
    out << +rbyte << ' ' << +gbyte << ' ' << +bbyte << '\n';
}


#endif