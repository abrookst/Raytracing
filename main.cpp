#include <iostream>
#include "color.h"
#include "point3.h"
#include "vector3.h"

int main() {

    // Image

    uint16_t imageWidth = 640;
    uint16_t imageHeight = 480;

    // Render

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (uint16_t j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (uint16_t i = 0; i < imageWidth; i++) {
            Color pixelColor = Color(float(i)/(imageWidth-1), float(j)/(imageHeight-1), 0.0f);
            write_color(std::cout, pixelColor);
        }
    }
    std::clog << "\rDone.                 " << std::endl;
}