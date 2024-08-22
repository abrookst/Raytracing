#include "common.h"
#include "vector3.h"
#include "sphere.h"
#include "camera.h"
#include "hittableList.h"
#include "material.h"

int main() {
    // World
    HittableList world;

    // Materials
    shared_ptr<Material> materialGround = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.02f);
    shared_ptr<Material> materialLarge = make_shared<Lambertian>(Color(0.55, 0.98, 0.95));

    shared_ptr<Material> materialSmall1 = make_shared<Metal>(Color(0.71, 0.43, 0.47), 0.0f);
    shared_ptr<Material> materialSmallest1 = make_shared<Metal>(Color(0.34, 0.61, 0.34), 0.5f);

    shared_ptr<Material> materialSmall2 = make_shared<Dielectric>(Color(0.34, 0.31, 0.54), 1.5f);
    shared_ptr<Material> materialSmallest2 = make_shared<Dielectric>(Color(0.34, 0.61, 0.34), 2.4f);

    
   

    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100, materialGround));
    world.add(make_shared<Sphere>(Point3( 0.0, 0.25, -1.5),   0.75, materialLarge));

    world.add(make_shared<Sphere>(Point3(-.9, -.25, -1.1),   0.25, materialSmall1));
    world.add(make_shared<Sphere>(Point3(-.45, -.4, -1),   0.1, materialSmallest1));
 
    world.add(make_shared<Sphere>(Point3(.9, -.2, -1.1),   0.3, materialSmall2));
    world.add(make_shared<Sphere>(Point3(.4, -.3, -.8),   0.2, materialSmallest2));
    

    // Camera
    Camera mainView;
    mainView.aspectRatio = 16.0f / 9.0f;
    mainView.imageWidth = 640;
    mainView.samplesPerPixel = 100;
    mainView.maxDepth = 10;
    mainView.fov = 90;

    mainView.render("main.ppm", world);
    mainView.fov = 60;
    mainView.render("second.ppm", world);

}