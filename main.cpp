#include <iostream>

int main() {

    // Image

    uint16_t image_width = 640;
    uint16_t image_height = 480;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (uint16_t j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (uint16_t i = 0; i < image_width; i++) {
            float r = float(i) / (image_width-1);
            float g = float(j) / (image_height-1);
            float b = 0.0;

            uint8_t ir = int(255 * r);
            uint8_t ig = int(255 * g);
            uint8_t ib = int(255 * b);

            std::cout << +ir << ' ' << +ig << ' ' << +ib << '\n';
        }
    }
    std::clog << "\rDone.                 " << std::endl;
}