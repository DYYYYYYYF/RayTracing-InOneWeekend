#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

//Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.141592653589793285;

//Utility Functions
inline double degress_to_radians(double degress) {
	return degress * pi / 180.0;
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}



//Common Headers
#include "ray.h"
#include "vec3.h"

#endif // !RTWEEKEND_H
