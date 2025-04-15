#ifndef COLOR_H
#define COLOR_H

#include "rtweekend.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel);
int convert_color(double pixel, int samples_per_pixel);

#endif

