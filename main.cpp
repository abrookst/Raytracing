#include "common.h"
#include "vector3.h"
#include "sphere.h"
#include "camera.h"
#include "hittableList.h"
#include "material.h"

int main() {
    // World
    HittableList world;

    shared_ptr<Material> materialGround = make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, materialGround));

    Interval heightInterval(0.2, .6);
    for (int a = -1; a < 4; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = random_float();
            Point3 center1(a + random_float(), 0.2, b + random_float());
            Point3 center2 = center1 + Vector3(0, random_float(0,.5), 0);
            shared_ptr<Material> sphereMaterial;

            if (chooseMat < 0.8) {
                // diffuse
                Color albedo = Color::random() * Color::random();
                sphereMaterial = make_shared<Lambertian>(albedo);
                world.add(make_shared<Sphere>(center1, center2, 0.2, sphereMaterial));
            } else if (chooseMat < 0.95) {
                // metal
                Color albedo = Color::random(0.5, 1);
                float fuzz = random_float(0, 0.5);
                sphereMaterial = make_shared<Metal>(albedo, fuzz);
                world.add(make_shared<Sphere>(center1, center2, 0.2, sphereMaterial));
            } else {
                // glass
                Color albedo = Color::random(0.5, 1);
                sphereMaterial = make_shared<Dielectric>(albedo, 1.5);
                world.add(make_shared<Sphere>(center1, center2, 0.2, sphereMaterial));
            }
        
        }
    }

    shared_ptr<Material> materialLarge1 = make_shared<Lambertian>(Color(0.2, 0.8, 0.3));
    world.add(make_shared<Sphere>(Point3(-2,2,-3.7), 2, materialLarge1));

    shared_ptr<Material> materialLarge2 = make_shared<Metal>(Color(0.8, 0.3, 0.2), 0.0);
    world.add(make_shared<Sphere>(Point3(-2,1.5,0), 1.5, materialLarge2));

    shared_ptr<Material> materialLarge3 = make_shared<Dielectric>(Color(0.3, 0.2, 0.8), 1.5);
    world.add(make_shared<Sphere>(Point3(-2,1,2.3), 1, materialLarge3));
    

    // Camera
    Camera mainView;
    mainView.aspectRatio = 16.0f / 9.0f;
    mainView.imageWidth = 400;
    mainView.samplesPerPixel = 100;
    mainView.maxDepth = 50;
    mainView.fov = 40;
    mainView.defocusAngle = 0.6;
    mainView.focusDist = 6.5;
    mainView.lookFrom = Point3(3,1,4);
    mainView.lookAt = Point3(-2,1.5,0);
    mainView.relativeUp = Vector3(0,1,0);
    mainView.render("main1.ppm", world);

    HittableList world2;

    // Materials
    shared_ptr<Material> materialLarge = make_shared<Lambertian>(Color(0.55, 0.98, 0.95));

    shared_ptr<Material> materialSmall1 = make_shared<Metal>(Color(0.71, 0.43, 0.47), 0.0f);
    shared_ptr<Material> materialSmallest1 = make_shared<Metal>(Color(0.34, 0.61, 0.34), 0.5f);

    shared_ptr<Material> materialSmall2 = make_shared<Dielectric>(Color(0.34, 0.31, 0.54), 1.5f);
    shared_ptr<Material> materialSmallest2 = make_shared<Dielectric>(Color(0.34, 0.61, 0.34), 2.4f);

    world2.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100, materialGround));
    world2.add(make_shared<Sphere>(Point3( 0.0, 0.25, -1.5),   0.75, materialLarge));

    world2.add(make_shared<Sphere>(Point3(-.9, -.25, -1.1),   0.25, materialSmall1));
    world2.add(make_shared<Sphere>(Point3(-.45, -.4, -1),   0.1, materialSmallest1));
 
    world2.add(make_shared<Sphere>(Point3(.9, -.2, -1.1),   0.3, materialSmall2));
    world2.add(make_shared<Sphere>(Point3(.4, -.3, -.8),   0.2, materialSmallest2));
    mainView.lookAt = Point3(0,0,-1);
    mainView.lookFrom = Point3(0,0,0);
    mainView.fov = 90;
    mainView.focusDist = 1;
    mainView.render("main2.ppm", world2);
}