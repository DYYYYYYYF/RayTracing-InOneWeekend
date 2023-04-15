#include <iostream>
#include <fstream>
#include "color.h"
#include "vec3.h"

int main(){

    const int WIDTH = 256;
    const int HEIGHT = 256;

    std::fstream fs;
    fs.open("../out.ppm", std::ios::ate | std::ios::out);
    fs << "P3\n" << WIDTH << ' ' << HEIGHT << "\n256\n";

    for(int i=0; i<HEIGHT; i++){
        for( int j=0; j<WIDTH; j++){
            rt::color pixel_color(double(i) / (WIDTH - 1), double(j) / (HEIGHT - 1), 1);
            rt::WriteColor(fs, pixel_color);
        }
    }

    return 0;
}
