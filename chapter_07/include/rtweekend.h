#pragma once

#include <limits>
#include <memory>
#include <cmath>

//Using
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constrants
const double infinty = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility Functions
inline double degress_to_radians(double degress){
    return degress * pi / 180;
}

inline double clamp(double x, double min, double max){
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

//Common Headers
#include "ray.h"
#include "vec3.h"
