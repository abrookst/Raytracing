#include "common.h"

#include "bvh.h"
#include "sphere.h"
#include "quad.h"
#include "camera.h"
#include "hittableList.h"
#include "material.h"
#include "texture.h"
#include "constant_medium.h"

void angled_balls()
{
    // World
    HittableList world;
    shared_ptr<Texture> checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));

    Interval heightInterval(0.2, .6);
    for (int a = -1; a < 4; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float chooseMat = random_float();
            Point3 center1(a + random_float(), 0.2, b + random_float());
            Point3 center2 = center1 + Vector3(0, random_float(0, .5), 0);
            shared_ptr<Material> sphereMaterial;

            if (chooseMat < 0.8)
            {
                // diffuse
                Color albedo = Color::random() * Color::random();
                sphereMaterial = make_shared<Lambertian>(albedo);
                world.add(make_shared<Sphere>(center1, center2, 0.2, sphereMaterial));
            }
            else if (chooseMat < 0.95)
            {
                // metal
                Color albedo = Color::random(0.5, 1);
                float fuzz = random_float(0, 0.5);
                sphereMaterial = make_shared<Metal>(albedo, fuzz);
                world.add(make_shared<Sphere>(center1, center2, 0.2, sphereMaterial));
            }
            else
            {
                // glass
                Color albedo = Color::random(0.5, 1);
                sphereMaterial = make_shared<Dielectric>(albedo, 1.5);
                world.add(make_shared<Sphere>(center1, center2, 0.2, sphereMaterial));
            }
        }
    }

    shared_ptr<Material> materialLarge1 = make_shared<Lambertian>(Color(0.2, 0.8, 0.3));
    world.add(make_shared<Sphere>(Point3(-2, 2, -3.7), 2, materialLarge1));

    shared_ptr<Material> materialLarge2 = make_shared<Metal>(Color(0.8, 0.3, 0.2), 0.0);
    world.add(make_shared<Sphere>(Point3(-2, 1.5, 0), 1.5, materialLarge2));

    shared_ptr<Material> materialLarge3 = make_shared<Dielectric>(Color(0.3, 0.2, 0.8), 1.5);
    world.add(make_shared<Sphere>(Point3(-2, 1, 2.3), 1, materialLarge3));

    // Camera
    Camera mainView;
    mainView.aspectRatio = 16.0f / 9.0f;
    mainView.imageWidth = 400;
    mainView.samplesPerPixel = 100;
    mainView.maxDepth = 50;
    mainView.fov = 40;
    mainView.defocusAngle = 0.6;
    mainView.focusDist = 6.5;
    mainView.lookFrom = Point3(3, 1, 4);
    mainView.lookAt = Point3(-2, 1.5, 0);
    mainView.relativeUp = Vector3(0, 1, 0);
    mainView.render("angled_balls.ppm", world);
}

void my_test()
{
    HittableList world2;
    shared_ptr<Texture> checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    // Materials
    shared_ptr<Material> materialLarge = make_shared<Lambertian>(Color(0.55, 0.98, 0.95));

    shared_ptr<Material> materialSmall1 = make_shared<Metal>(Color(0.71, 0.43, 0.47), 0.0f);
    shared_ptr<Material> materialSmallest1 = make_shared<Metal>(Color(0.34, 0.61, 0.34), 0.5f);

    shared_ptr<Material> materialSmall2 = make_shared<Dielectric>(Color(0.34, 0.31, 0.54), 1.5f);
    shared_ptr<Material> materialSmallest2 = make_shared<Dielectric>(Color(0.34, 0.61, 0.34), 2.4f);

    world2.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, make_shared<Lambertian>(checker)));
    world2.add(make_shared<Sphere>(Point3(0.0, 0.25, -1.5), 0.75, materialLarge));

    world2.add(make_shared<Sphere>(Point3(-.9, -.25, -1.1), 0.25, materialSmall1));
    world2.add(make_shared<Sphere>(Point3(-.45, -.4, -1), 0.1, materialSmallest1));

    world2.add(make_shared<Sphere>(Point3(.9, -.2, -1.1), 0.3, materialSmall2));
    world2.add(make_shared<Sphere>(Point3(.4, -.3, -.8), 0.2, materialSmallest2));

    Camera mainView;
    mainView.aspectRatio = 16.0f / 9.0f;
    mainView.imageWidth = 400;
    mainView.samplesPerPixel = 100;
    mainView.maxDepth = 50;
    mainView.fov = 40;
    mainView.defocusAngle = 0.6;
    mainView.focusDist = 6.5;
    mainView.lookFrom = Point3(3, 1, 4);
    mainView.lookAt = Point3(-2, 1.5, 0);
    mainView.relativeUp = Vector3(0, 1, 0);
    mainView.lookAt = Point3(0, 0, -1);
    mainView.lookFrom = Point3(0, 0, 0);
    mainView.fov = 90;
    mainView.focusDist = 1;

    mainView.render("my_test.ppm", world2);
}

void texture_test()
{
    HittableList world;
    shared_ptr<Texture> checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));
    shared_ptr<Texture> texture = make_shared<ImageTexture>("mars.jpg");
    shared_ptr<Material> matLambertian = make_shared<Lambertian>(texture);
    shared_ptr<Material> matMetal = make_shared<Metal>(texture, 0.0f);
    shared_ptr<Material> matDielectric = make_shared<Dielectric>(texture, 1.5f);
    world.add(make_shared<Sphere>(Point3(0.0, 2, 0), 2, matLambertian));
    world.add(make_shared<Sphere>(Point3(-4, 2, 0), 2, matMetal));
    world.add(make_shared<Sphere>(Point3(4, 2, 0), 2, matDielectric));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 800;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.fov = 40;
    cam.lookFrom = Point3(0, 5, 12);
    cam.lookAt = Point3(0, 0, 0);
    cam.relativeUp = Vector3(0, 1, 0);

    cam.defocusAngle = 0;
    cam.render("texture_test.ppm", world);
}

void noise_sphere()
{
    HittableList world;
    shared_ptr<Texture> perlinTexture = make_shared<NoiseTexture>(4);

    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlinTexture)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlinTexture)));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.fov = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.relativeUp = Vector3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render("noise_sphere.ppm", world);
}

void cornell_box()
{
    HittableList world;

    shared_ptr<Material> red = make_shared<Lambertian>(Color(.65, .05, .05));
    shared_ptr<Material> white = make_shared<Lambertian>(Color(.73, .73, .73));
    shared_ptr<Material> green = make_shared<Lambertian>(Color(.12, .45, .15));
    shared_ptr<Material> light = make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(make_shared<Quad>(Point3(555, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point3(0, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point3(343, 554, 332), Vector3(-130, 0, 0), Vector3(0, 0, -105), light));
    world.add(make_shared<Quad>(Point3(0, 0, 0), Vector3(555, 0, 0), Vector3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point3(555, 555, 555), Vector3(-555, 0, 0), Vector3(0, 0, -555), white));
    world.add(make_shared<Quad>(Point3(0, 0, 555), Vector3(555, 0, 0), Vector3(0, 555, 0), white));

    shared_ptr<Hittable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = rotate(box1, 10, 15, 0);
    box1 = make_shared<Translate>(box1, Vector3(265, 0, 295));
    world.add(box1);

    shared_ptr<Hittable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = rotate(box2, 0, -18, -10);
    box2 = make_shared<Translate>(box2, Vector3(130, 0, 65));
    world.add(box2);

    Camera cam;

    cam.aspectRatio = 1.0;
    cam.imageWidth = 600;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 10;
    cam.background = Color(0, 0, 0);

    cam.fov = 40;
    cam.lookFrom = Point3(278, 278, -800);
    cam.lookAt = Point3(278, 278, 0);
    cam.relativeUp = Vector3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render("cornell.ppm", world);
}

void final_scene()
{
    HittableList world;

    shared_ptr<Texture> ground = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));

    int boxesPerSide = 20;
    for (int i = 0; i < boxesPerSide; i++)
    {
        for (int j = 0; j < boxesPerSide; j++)
        {
            float w = 100.0;
            float x0 = -1000.0 + i * w;
            float y0 = 0.0;
            float z0 = -1000.0 + j * w;

            float x1 = x0 + w;
            float y1 = random_float(1, 50);
            float z1 = z0 + w;

            world.add(Box(Point3(x0, y0, z0), Point3(x1, y1, z1), make_shared<Lambertian>(ground)));
        }
    }

    shared_ptr<Material> light = make_shared<DiffuseLight>(Color(7, 7, 7));
    world.add(make_shared<Quad>(Point3(123, 554, 147), Vector3(300, 0, 0), Vector3(0, 0, 265), light));

    shared_ptr<Texture> marble = make_shared<ImageTexture>("marble.jpg");
    world.add(make_shared<Sphere>(Point3(230, 100, 90), 70, make_shared<Dielectric>(marble, 1.5)));

    HittableList boxes2;
    shared_ptr<Material> white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(Point3::random(0,165), 10, white));
    }

    world.add(make_shared<Translate>(rotate(make_shared<BVHNode>(boxes2), 0,15,0), Vector3(-200,50,315)));

    shared_ptr<Hittable> box1 = Box(Point3(330, 100, 120), Point3(530, 300, 420), make_shared<Lambertian>(marble));
    world.add(make_shared<ConstantMedium>(box1, 0.001, Color(1, 1, 1)));
    shared_ptr<Texture> mars = make_shared<ImageTexture>("mars.jpg");
    world.add(make_shared<Sphere>(Point3(430, 200, 320), 100, make_shared<Lambertian>(mars)));

    world.add(make_shared<Sphere>(Point3(630, 400, 320), Point3(630, 350, 320), 35, make_shared<Metal>(Color(0.8, 0.3, 0.2), 0.0)));

    shared_ptr<Texture> pertext = make_shared<NoiseTexture>(0.2);
    world.add(make_shared<Sphere>(Point3(220,380,300), 100, make_shared<Lambertian>(pertext)));

    shared_ptr<Texture> crate = make_shared<ImageTexture>("crate.png");
    shared_ptr<Hittable> box2 = Box(Point3(-670, 550, 20), Point3(-370, 850, 320), make_shared<Lambertian>(crate));
    world.add(rotate(box2, 30, 15, 2));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 1920;
    cam.samplesPerPixel = 500;
    cam.maxDepth = 10;
    cam.background = Color(0, 0, 0);

    cam.fov = 40;
    cam.lookFrom = Point3(478, 278, -600);
    cam.lookAt = Point3(278, 278, 0);
    cam.relativeUp = Vector3(0, 1, 0);

    cam.defocusAngle = 0;

    cam.render("final.ppm", world);
}

int main()
{
    switch (6)
    {
    case 1:
        angled_balls();
        break;
    case 2:
        my_test();
        break;
    case 3:
        texture_test();
        break;
    case 4:
        noise_sphere();
        break;
    case 5:
        cornell_box();
        break;
    case 6:
        final_scene();
        break;
    }
}