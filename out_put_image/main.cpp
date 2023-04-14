#include <iostream>
#include <fstream>

int main(){

    const int WIDTH = 256;
    const int HEIGHT = 256;

    std::fstream fs;
    fs.open("../out.ppm", std::ios::ate | std::ios::out);
    fs << "P3\n" << WIDTH << ' ' << HEIGHT << "\n256\n";

    for(int i=0; i<HEIGHT; i++){
        for( int j=0; j<WIDTH; j++){
            auto r = double(i) / (WIDTH - 1);
            auto g = double(j) / (HEIGHT - 1);
            auto b = 1;

            int ir = static_cast<int>(r * 255.999);
            int ig = static_cast<int>(g * 255.999);
            int ib = static_cast<int>(b * 255.999);

            fs << ir << ' ' << ig << ' ' << ib << std::endl;
        }
    }

    return 0;
}
