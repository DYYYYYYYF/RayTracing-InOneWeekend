#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

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

    //world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100.0));

    //Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    
    std::fstream fs;
    fs.open("out.ppm", std::ios::ate | std::ios::out);
    fs << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for( int j=HEIGHT; j>=0; j--){
        for(int i=0; i<WIDTH; ++i){
            auto u = double(i) / (WIDTH - 1);
            auto v = double(j) / (HEIGHT - 1);
            ray r(origin, lower_left_corner + v*vertical + u * horizontal - origin);
            color pixel_color = ray_color(r, world);
            WriteColor(fs, pixel_color);
        }
    }

    return 0;
}
