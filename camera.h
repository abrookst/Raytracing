#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "bvh.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include <fstream>

class Camera
{
public:
    float aspectRatio = 1.0f;
    uint16_t imageWidth = 100;
    uint16_t samplesPerPixel = 10;
    uint16_t maxDepth = 10;
    Color background = Color(0.70, 0.80, 1.00);

    float fov = 90;
    Point3 lookFrom = Point3(0,0,0);
    Point3 lookAt = Point3(0,0,-1);
    Vector3 relativeUp = Vector3(0,1,0);

    float defocusAngle = 0;
    float focusDist = 10;


    void render(const std::string filename, HittableList& world)
    {
        world = HittableList(make_shared<BVHNode>(world));
        initialize();

        std::ofstream ofs(filename, std::ios::binary);

        ofs << "P3\n"
            << imageWidth << ' ' << imageHeight << "\n255\n";

        for (uint16_t j = 0; j < imageHeight; j++)
        {
            std::clog << "\rScanlines remaining for " << filename << ": " << (imageHeight - j) << ' ' << std::flush;
            for (uint16_t i = 0; i < imageWidth; i++)
            {
                Color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++)
                {
                    Ray r = get_ray(i, j);
                    pixelColor += ray_color(r, maxDepth, world);
                }
                write_color(ofs, pixelSamplesScale * pixelColor);
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
        Vector3 pixelSample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);

        Point3 rayOrigin = (defocusAngle <= 0) ? cameraCenter : defocus_disk_sample();
        Vector3 rayDirection = pixelSample - rayOrigin;
        float rayTime = random_float();

        return Ray(rayOrigin, rayDirection, rayTime);
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

    Color ray_color(const Ray &ray, uint16_t depth, Hittable &world)
    {
        if (depth <= 0)
        {
            return Color(0, 0, 0);
        }

        HitRecord rec;
        if (!world.hit(ray, Interval(0.001, infinity), rec))
            return background;

        Ray scattered;
        Color attenuation;
        Color colorFromEmission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(ray, rec, attenuation, scattered))
            return colorFromEmission;

        Color colorFromScatter = attenuation * ray_color(scattered, depth-1, world);

        return colorFromEmission + colorFromScatter;
    }
};

#endif