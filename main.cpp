#include "common.h"
#include "vector3.h"
#include "sphere.h"
#include "hittableList.h"

Color ray_color(Ray& ray, const Hittable& world){ 
    HitRecord rec;
    if (world.hit(ray, 0, std::numeric_limits<float>::infinity(), rec)) {
        return 0.5 * (rec.normal + Color(1,1,1));
    }

    Vector3 unitDirection = unit_vector(ray.direction());
    float   a = 0.5*(unitDirection.y() + 1.0);
    return (1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
 }

int main() {

    // Image
    float aspectRatio = 4.0f / 3.0f;
    uint16_t imageWidth = 640;
    uint16_t imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    // World
    HittableList world;

    world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100));

    // Camera 
    float focalLength = 1.0;
    float viewportHeight = 2.0;
    float viewportWidth = viewportHeight * (float(imageWidth)/imageHeight);
    Point3 cameraCenter = Point3(0,0,0);

    // Viewport
    Vector3 viewportU = Vector3(viewportWidth, 0, 0);
    Vector3 viewportV = Vector3(0, -viewportHeight, 0);

    // Pixel Delta
    Vector3 pixelDeltaU = viewportU / imageWidth;
    Vector3 pixelDeltaV = viewportV / imageHeight;

    // starting pixel
    Point3 viewportUpperLeft = cameraCenter - Vector3(0, 0, focalLength) - viewportU/2 - viewportV/2;
    Point3 pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    // Render

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (uint16_t j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (uint16_t i = 0; i < imageWidth; i++) {
            Point3 pixelCenter = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
            Point3 rayDirection = pixelCenter - cameraCenter;
            Ray r(cameraCenter, rayDirection);

            Color pixelColor = ray_color(r, world);
            write_color(std::cout, pixelColor);
        }
    }
    std::clog << "\rDone.                 " << std::endl;
}