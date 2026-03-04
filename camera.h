#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "bvh.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include <vector>

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


    void render(HittableList& world, std::vector<unsigned char>& pixelBuffer)
    {
        world = HittableList(make_shared<BVHNode>(world));
        initialize();

        pixelBuffer.resize(imageWidth * imageHeight * 3);

        for (uint16_t j = 0; j < imageHeight; j++)
        {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            for (uint16_t i = 0; i < imageWidth; i++)
            {
                Color pixelColor(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; sample++)
                {
                    Ray r = get_ray(i, j);
                    pixelColor += ray_color(r, maxDepth, world);
                }
                 write_color(pixelBuffer, i, j, pixelSamplesScale * pixelColor);
            }
        }
        std::clog << "\rRender has been completed." << std::endl;
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
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;
        pixelSamplesScale = 1.0f / samplesPerPixel;
        cameraCenter = lookFrom;
        float theta = degrees_to_radians(fov);
        float h = std::tan(theta / 2);
        float viewportHeight = 2.0 * h * focusDist;
        float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);
        w = unit_vector(lookFrom - lookAt);
        u = unit_vector(cross(relativeUp,w));
        v = cross(w,u);
        Vector3 viewportU = viewportWidth * u;
        Vector3 viewportV = viewportHeight * -v;
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;
        Point3 viewportUpperLeft = cameraCenter - (focusDist * w) - viewportU / 2 - viewportV / 2;
        pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
        float defocusRadius = focusDist * std::tan(degrees_to_radians(defocusAngle / 2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusDiskU;
    }

    void write_color(std::vector<unsigned char>& pixelBuffer, int i, int j, Color pixelColor) {
        int index = (j * imageWidth + i) * 3;
        pixelBuffer[index] = static_cast<unsigned char>(255.999 * pixelColor.x());
        pixelBuffer[index + 1] = static_cast<unsigned char>(255.999 * pixelColor.y());
        pixelBuffer[index + 2] = static_cast<unsigned char>(255.999 * pixelColor.z());
    }

    Ray get_ray(uint16_t i, uint16_t j) const
    {
        Vector3 offset = sample_square();
        Vector3 pixelSample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);
        Point3 rayOrigin = (defocusAngle <= 0) ? cameraCenter : defocus_disk_sample();
        Vector3 rayDirection = pixelSample - rayOrigin;
        float rayTime = random_float();
        return Ray(rayOrigin, rayDirection, rayTime);
    }
    Vector3 sample_square() const
    {
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