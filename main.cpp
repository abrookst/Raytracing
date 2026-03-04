#include "common.h"
#include "bvh.h"
#include "sphere.h"
#include "quad.h"
#include "camera.h"
#include "hittableList.h"
#include "material.h"
#include "texture.h"
#include "constant_medium.h"

#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

void angled_balls(HittableList& world)
{
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
}

void my_test(HittableList& world)
{
    shared_ptr<Texture> checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    // Materials
    shared_ptr<Material> materialLarge = make_shared<Lambertian>(Color(0.55, 0.98, 0.95));

    shared_ptr<Material> materialSmall1 = make_shared<Metal>(Color(0.71, 0.43, 0.47), 0.0f);
    shared_ptr<Material> materialSmallest1 = make_shared<Metal>(Color(0.34, 0.61, 0.34), 0.5f);

    shared_ptr<Material> materialSmall2 = make_shared<Dielectric>(Color(0.34, 0.31, 0.54), 1.5f);
    shared_ptr<Material> materialSmallest2 = make_shared<Dielectric>(Color(0.34, 0.61, 0.34), 2.4f);

    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Point3(0.0, 0.25, -1.5), 0.75, materialLarge));

    world.add(make_shared<Sphere>(Point3(-.9, -.25, -1.1), 0.25, materialSmall1));
    world.add(make_shared<Sphere>(Point3(-.45, -.4, -1), 0.1, materialSmallest1));

    world.add(make_shared<Sphere>(Point3(.9, -.2, -1.1), 0.3, materialSmall2));
    world.add(make_shared<Sphere>(Point3(.4, -.3, -.8), 0.2, materialSmallest2));
}

void texture_test(HittableList& world)
{
    shared_ptr<Texture> checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));
    shared_ptr<Texture> texture = make_shared<ImageTexture>("mars.jpg");
    shared_ptr<Material> matLambertian = make_shared<Lambertian>(texture);
    shared_ptr<Material> matMetal = make_shared<Metal>(texture, 0.0f);
    shared_ptr<Material> matDielectric = make_shared<Dielectric>(texture, 1.5f);
    world.add(make_shared<Sphere>(Point3(0.0, 2, 0), 2, matLambertian));
    world.add(make_shared<Sphere>(Point3(-4, 2, 0), 2, matMetal));
    world.add(make_shared<Sphere>(Point3(4, 2, 0), 2, matDielectric));
}

void noise_sphere(HittableList& world)
{
    shared_ptr<Texture> perlinTexture = make_shared<NoiseTexture>(4);

    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlinTexture)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlinTexture)));
}

void cornell_box(HittableList& world)
{
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
}

void final_scene(HittableList& world)
{
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

    world.add(make_shared<Sphere>(Point3(630, 400, 320), Point3(630, 350, 320), 70, make_shared<Metal>(Color(0.8, 0.3, 0.2), 0.0)));

    shared_ptr<Texture> pertext = make_shared<NoiseTexture>(0.2);
    world.add(make_shared<Sphere>(Point3(220,380,300), 100, make_shared<Lambertian>(pertext)));

    shared_ptr<Texture> crate = make_shared<ImageTexture>("crate.png");
    shared_ptr<Hittable> box2 = Box(Point3(-670, 550, 20), Point3(-370, 850, 320), make_shared<Lambertian>(crate));
    world.add(rotate(box2, 30, 15, 2));
}


int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create window with SDL_Renderer graphics context
    SDL_Window* window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Resolution presets (width only, height adapts to aspect ratio)
    struct Resolution {
        const char* name;
        int width;
    };
    
    const Resolution resolutions[] = {
        {"Low", 800},
        {"Medium", 1280},
        {"High", 1920},
        {"Very High", 2560},
        {"Ultra", 3840}
    };
    int currentResolution = 1; // Default to 1280px

    Camera cam;
    cam.imageWidth = resolutions[currentResolution].width;
    HittableList world;
    std::vector<unsigned char> pixelBuffer;
    SDL_Texture* texture = nullptr;

    int currentScene = 1;

    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // ImGui Window
        ImGui::Begin("Settings");
        ImGui::Text("Select Scene:");
        ImGui::RadioButton("Angled Balls", &currentScene, 1);
        ImGui::RadioButton("My Test", &currentScene, 2);
        ImGui::RadioButton("Texture Test", &currentScene, 3);
        ImGui::RadioButton("Noise Sphere", &currentScene, 4);
        ImGui::RadioButton("Cornell Box", &currentScene, 5);
        ImGui::RadioButton("Final Scene", &currentScene, 6);

        ImGui::Separator();

        ImGui::Text("Camera Settings:");
        int samplesPerPixel = static_cast<int>(cam.samplesPerPixel);
        int maxDepth = static_cast<int>(cam.maxDepth);
        if (ImGui::SliderInt("Samples per Pixel", &samplesPerPixel, 1, 1000))
            cam.samplesPerPixel = static_cast<uint16_t>(samplesPerPixel);
        if (ImGui::SliderInt("Max Depth", &maxDepth, 1, 50))
            cam.maxDepth = static_cast<uint16_t>(maxDepth);
        ImGui::SliderFloat("FOV", &cam.fov, 1, 180);
        ImGui::SliderFloat("Defocus Angle", &cam.defocusAngle, 0, 10);
        ImGui::SliderFloat("Focus Distance", &cam.focusDist, 0.1f, 20);

        ImGui::Separator();
        ImGui::Text("Render Width:");
        if (ImGui::Combo("##Resolution", &currentResolution, [](void* data, int idx, const char** out_text) {
            auto res = reinterpret_cast<const Resolution*>(data);
            *out_text = res[idx].name;
            return true;
        }, (void*)resolutions, IM_ARRAYSIZE(resolutions)))
        {
            // Update width when resolution changes
            cam.imageWidth = resolutions[currentResolution].width;
        }
        
        // Display current dimensions
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        cam.aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
        int renderHeight = static_cast<int>(cam.imageWidth / cam.aspectRatio);
        ImGui::Text("Render Dimensions: %dx%d (%.2f:1)", cam.imageWidth, renderHeight, cam.aspectRatio);

        if (ImGui::Button("Render"))
        {
            world.clear();
            switch (currentScene)
            {
                case 1: // angled_balls
                    cam.lookFrom = Point3(13,2,3);
                    cam.lookAt = Point3(0,0,0);
                    cam.fov = 20;
                    cam.defocusAngle = 0.6;
                    cam.focusDist = 10.0;
                    angled_balls(world); 
                    break;
                case 2: // my_test
                    cam.lookFrom = Point3(0,0,0);
                    cam.lookAt = Point3(0,0,-1);
                    cam.fov = 90;
                    cam.defocusAngle = 0;
                    cam.focusDist = 10;
                    my_test(world); 
                    break;
                case 3: // texture_test
                    cam.lookFrom = Point3(13,2,3);
                    cam.lookAt = Point3(0,0,0);
                    cam.fov = 20;
                    cam.defocusAngle = 0;
                    cam.focusDist = 10;
                    texture_test(world); 
                    break;
                case 4: // noise_sphere
                    cam.lookFrom = Point3(13,2,3);
                    cam.lookAt = Point3(0,0,0);
                    cam.fov = 20;
                    cam.defocusAngle = 0;
                    cam.focusDist = 10;
                    noise_sphere(world); 
                    break;
                case 5: // cornell_box
                    cam.lookFrom = Point3(278, 278, -800);
                    cam.lookAt = Point3(278, 278, 0);
                    cam.fov = 40;
                    cam.defocusAngle = 0;
                    cam.focusDist = 10;
                    cornell_box(world); 
                    break;
                case 6: // final_scene
                    cam.lookFrom = Point3(478, 278, -600);
                    cam.lookAt = Point3(278, 278, 0);
                    cam.fov = 40;
                    cam.defocusAngle = 0;
                    cam.focusDist = 10;
                    final_scene(world); 
                    break;
            }

            // Get current window size for aspect ratio
            int windowWidth, windowHeight;
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            cam.aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
            
            cam.render(world, pixelBuffer);

            if (texture) SDL_DestroyTexture(texture);
            int renderHeight = static_cast<int>(cam.imageWidth / cam.aspectRatio);
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, cam.imageWidth, renderHeight);
            SDL_UpdateTexture(texture, NULL, pixelBuffer.data(), cam.imageWidth * 3);
        }
        ImGui::SameLine();
        if (ImGui::Button("Save to PPM") && texture)
        {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::tm buf;
            localtime_s(&buf, &in_time_t);
            std::stringstream ss;
            ss << "../../..//render-" << std::put_time(&buf, "%Y%m%d-%H%M%S") << ".ppm";
            std::string filename = ss.str();

            std::ofstream ppm_file(filename);
            int renderHeight = static_cast<int>(cam.imageWidth / cam.aspectRatio);
            ppm_file << "P3\n" << cam.imageWidth << ' ' << renderHeight << "\n255\n";
            for (int i = 0; i < cam.imageWidth * renderHeight; ++i) {
                ppm_file << static_cast<int>(pixelBuffer[i*3+0]) << ' '
                         << static_cast<int>(pixelBuffer[i*3+1]) << ' '
                         << static_cast<int>(pixelBuffer[i*3+2]) << '\n';
            }
            ppm_file.close();
            std::cout << "Saved render to " << ss.str() << std::endl;
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 114, 144, 154, 255);
        SDL_RenderClear(renderer);

        if (texture)
        {
            SDL_RenderCopy(renderer, texture, NULL, NULL);
        }

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}