#include "Camera.h"
#include "World.h"

#include <iostream>
#include <fstream>

void write_color_to_file(std::ostream &out, Vector3D pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;
    auto scale = 1.0 / samples_per_pixel;
    r = clamp(256 * sqrt(scale * r), 0, 255);
    g = clamp(256 * sqrt(scale * g), 0, 255);
    b = clamp(256 * sqrt(scale * b), 0, 255);
    out << int(r) << ' ' << int(g) << ' ' << int(b)<< '\n';
}

Vector3D ray_hit_color(Ray& r, World& world, int max_light_bounce_num)
{
    if (max_light_bounce_num <= 0)
        return Vector3D(0,0,0);
    
    HitResult hit = world.hit(r, 0.001, std::numeric_limits<float>::infinity());
    if (hit.m_isHit)
    {
        ReflectResult res = hit.m_hitMaterial->reflect(r, hit);
        return res.m_color * ray_hit_color(res.m_ray, world, max_light_bounce_num-1);
    }
    return Vector3D(1, 1, 1);
}

int main()
{
    int width =  768;
    int height = 540;
    float aspect_ratio = width / float(height);
    int rays_per_pixel = 100;
    const int max_light_bounce_num = 5;
    
    Vector3D eye(20,3,3);
    Vector3D target(0,0,0);
    Vector3D up(0,1,0);
    float fov = 20;//degree
    Camera camera(eye, target, up, fov, aspect_ratio);
    
    World world;
    
    // TODO: 6. uncomment one by one and render the following worlds
    // world.generate_scene_one_diffuse();
    // world.generate_scene_one_specular();
    // world.generate_scene_multi_diffuse();
    // world.generate_scene_multi_specular();
    world.generate_scene_all();
   
    //TODO: 1. set your own path for output image
    std::string result_ppm_path = "C:/Users/Corinna/Documents/painge/assignment 4/ppms/all.ppm";
    
    std::ofstream fout (result_ppm_path);
    fout << "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = height-1; j >= 0; --j)
    {
        std::cout << "casting row " << j << std::endl;
        for (int i = 0; i < width; ++i)
        {
            Vector3D pixel_color(0,0,0);
            for (int s = 0; s < rays_per_pixel; ++s)
            {
                float col = (i + random_float()) / (width-1);
                float row = (j + random_float()) / (height-1);
                Ray r = camera.generate_ray(col, row);
                pixel_color += ray_hit_color(r, world, max_light_bounce_num);
            }
            write_color_to_file(fout, pixel_color, rays_per_pixel);
        }
    }

    std::cout << "raytracing done!" << std::endl << "ppm saved at " << result_ppm_path << std::endl;
}

