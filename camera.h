#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "hittable.h"
#include "material.h"
#include <fstream>

class Camera
{
public:
    float aspectRatio = 1.0f;
    uint16_t imageWidth = 100;
    uint16_t samplesPerPixel = 10;
    uint16_t maxDepth = 10;

    float fov = 90;
    Point3 lookFrom = Point3(0,0,0);
    Point3 lookAt = Point3(0,0,-1);
    Vector3 relativeUp = Vector3(0,1,0);

    float defocusAngle = 0;
    float focusDist = 10;


    void render(const std::string filename, const Hittable &world)
    {
        initialize();

        std::ofstream ofs(filename, std::ios::binary);

        ofs << "P3\n"
            << imageWidth << ' ' << imageHeight << "\n255\n";

        for (uint16_t j = 0; j < imageHeight; j++)
        {
            std::clog << "\rScanlines remaining for " << filename << ": " << (imageHeight - j) << ' ' << std::flush;
            for (uint16_t i = 0; i < imageWidth; i++)
            {
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++)
                {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, maxDepth, world);
                }
                write_color(ofs, pixelSamplesScale * pixel_color);
            }
        }
        std::clog << "\rRender for " << filename << " has been completed." << std::endl;
    }

private:
    uint16_t imageHeight;
    float pixelSamplesScale;
    Point3 cameraCenter;
    Point3 pixel00Loc;
    Vector3 pixelDeltaU;
    Vector3 pixelDeltaV;
    Vector3 u,v,w;
    Vector3 defocusDiskU;
    Vector3 defocusDiskV;

    void initialize()
    {
        // Image
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;
        pixelSamplesScale = 1.0f / samplesPerPixel;

        // Camera
        cameraCenter = lookFrom;
        float theta = degrees_to_radians(fov);
        float h = std::tan(theta / 2);
        float viewportHeight = 2.0 * h * focusDist;
        float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);

        w = unit_vector(lookFrom - lookAt);
        u = unit_vector(cross(relativeUp,w));
        v = cross(w,u);

        // Viewport
        Vector3 viewportU = viewportWidth * u;
        Vector3 viewportV = viewportHeight * -v;

        // Pixel Delta
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;
        // starting pixel
        Point3 viewportUpperLeft = cameraCenter - (focusDist * w) - viewportU / 2 - viewportV / 2;
        pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        float defocusRadius = focusDist * std::tan(degrees_to_radians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    Ray get_ray(uint16_t i, uint16_t j) const
    {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        Vector3 offset = sample_square();
        Vector3 pixel_sample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);

        Point3 ray_origin = (defocusAngle <= 0) ? cameraCenter : defocus_disk_sample();
        Vector3 ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

    Vector3 sample_square() const
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return Vector3(random_float() - 0.5, random_float() - 0.5, 0);
    }

    Point3 defocus_disk_sample() const {
        Vector3 p = random_in_unit_disk();
        return cameraCenter + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
    }

    Color ray_color(const Ray &ray, uint16_t depth, const Hittable &world)
    {
        if (depth <= 0)
        {
            return Color(0, 0, 0);
        }

        HitRecord rec;
        if (world.hit(ray, Interval(0.001, infinity), rec))
        {
            Ray scattered;
            Color attenuation;
            if (rec.mat->scatter(ray, rec, attenuation, scattered))
            {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return Color(0, 0, 0);
        }

        Vector3 unitDirection = unit_vector(ray.direction());
        float a = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};

#endif