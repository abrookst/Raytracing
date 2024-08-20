#include "common.h"
#include "vector3.h"
#include "sphere.h"
#include "camera.h"
#include "hittableList.h"

int main() {

    // Image
    


    // World
    HittableList world;

    world.add(make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(Point3(0,-100.5,-1), 100));

    // Camera
    Camera mainView;
    mainView.aspectRatio = 4.0f / 3.0f;
    mainView.imageWidth = 640;

    mainView.render(world);

}