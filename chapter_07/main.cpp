#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>
#include <fstream>

color ray_color(const ray& r, const hittable& world){
    hit_record rec;
    if(world.hit(r, 0.001, infinty, rec)){
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(){

    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int WIDTH = 800;
    const int HEIGHT = static_cast<int>(WIDTH / aspect_ratio);
    const int samples_per_pixel = 100;

    //world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100.0));

    camera cam;
   
    std::fstream fs;
    fs.open("out.ppm", std::ios::ate | std::ios::out);
    fs << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for( int j=HEIGHT; j>=0; j--){
        for(int i=0; i<WIDTH; ++i){
            color pixel_color(0, 0, 0);
            for(int x=0; x<samples_per_pixel; x++){
                auto u = (i + random_double()) / (WIDTH - 1);
                auto v = (j + random_double()) / (HEIGHT - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }

           WriteColor(fs, pixel_color, samples_per_pixel);
        }
    }

    return 0;
}
