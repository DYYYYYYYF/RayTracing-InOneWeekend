#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:
	camera();
	camera(double vfov, double aspect_radio, point3 lookfrom, 
		point3 lookat, vec3 vup, double aperture = 0.0, double focus_dist = 1.0);
	ray get_ray(double s, double t) const;
	
public:
	point3 origin;
	point3 lower_left_corner;
	vec3 horization;
	vec3 vertical;

	vec3 u, v, w;
	double lens_radius;

};

#endif // !CAMERA_H
