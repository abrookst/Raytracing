#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "hittable.h"

class Camera
{
public:
    float aspectRatio = 1.0f;
    uint16_t imageWidth = 100;

    void render(const Hittable &world)
    {
        initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
        std::clog << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (uint16_t j = 0; j < imageHeight; j++)
        {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            for (uint16_t i = 0; i < imageWidth; i++)
            {
                Point3 pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);

                Point3 rayDirection = pixelCenter - cameraCenter;
                Ray r(cameraCenter, rayDirection);

                Color pixelColor = ray_color(r, world);
                write_color(std::cout, pixelColor);
            }
        }
        std::clog << "\rDone.                 " << std::endl;
    }

private:
    uint16_t imageHeight;
    Point3 cameraCenter;
    Point3 pixel00Loc;
    Vector3 pixelDeltaU;
    Vector3 pixelDeltaV;

    void initialize()
    {
        // Image
        imageHeight = int(imageWidth / aspectRatio);
        std::clog << imageHeight << std::endl;
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        // Camera
        cameraCenter = Point3(0, 0, 0);
        float focalLength = 1.0;
        float viewportHeight = 2.0;
        float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);

        // Viewport
        Vector3 viewportU = Vector3(viewportWidth, 0, 0);
        Vector3 viewportV = Vector3(0, -viewportHeight, 0);

        // Pixel Delta
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;
        // starting pixel
        Point3 viewportUpperLeft = cameraCenter - Vector3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
        pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
    }

    Color ray_color(Ray &ray, const Hittable &world)
    {
        HitRecord rec;
        if (world.hit(ray, Interval(0, infinity), rec))
        {
            return 0.5 * (rec.normal + Color(1, 1, 1));
        }

        Vector3 unitDirection = unit_vector(ray.direction());
        float a = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};

#endif