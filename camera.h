#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "hittable.h"

class Camera
{
public:
    float aspectRatio = 1.0f;
    uint16_t imageWidth = 100;
    uint16_t samplesPerPixel = 10;

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
                Color pixel_color(0,0,0);
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }
                write_color(std::cout, pixelSamplesScale * pixel_color);
            }
        }
        std::clog << "\rDone.                 " << std::endl;
    }

private:
    uint16_t imageHeight;
    float pixelSamplesScale;
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
        pixelSamplesScale = 1.0f / samplesPerPixel;

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

    Ray get_ray(uint16_t i, uint16_t j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00Loc
                          + ((i + offset.x()) * pixelDeltaU)
                          + ((j + offset.y()) * pixelDeltaV);

        auto ray_origin = cameraCenter;
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    Vector3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return Vector3(random_float() - 0.5, random_float() - 0.5, 0);
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