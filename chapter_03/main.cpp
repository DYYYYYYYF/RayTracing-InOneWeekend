#include <iostream>
#include <fstream>
#include "color.h"
#include "vec3.h"
#include "ray.h"

color ray_color(const ray& r){
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(){

    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int WIDTH = 400;
    const int HEIGHT = static_cast<int>(WIDTH / aspect_ratio);

    //Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    
    std::fstream fs;
    fs.open("../out.ppm", std::ios::ate | std::ios::out);
    fs << "P3\n" << WIDTH << ' ' << HEIGHT << "\n256\n";

    for(int i=0; i<HEIGHT; i++){
        for( int j=0; j<WIDTH; j++){
            auto u = double(i) / (WIDTH - 1);
            auto v = double(j) / (HEIGHT - 1);
            ray r(origin, lower_left_corner + u*horizontal + v * vertical);
            color pixel_color = ray_color(r);
            WriteColor(fs, pixel_color);
        }
    }

    return 0;
}
