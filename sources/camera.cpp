#include "camera.h"

camera::camera() {
	auto aspect_ratio = 16.0 / 9.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * aspect_ratio;
	auto focal_length = 1.0;

	origin = point3(0, 0, 0);
	horization = vec3(viewport_width, 0, 0);
	vertical = vec3(0, viewport_height, 0);
	lower_left_corner = origin - horization / 2 - vertical / 2 - vec3(0, 0, focal_length);

}

camera::camera(double vfov, double aspect_radio, point3 lookfrom, 
	point3 lookat, vec3 vup, double aperture, double focus_dist) {
	auto theta = degress_to_radians(vfov);
	auto half_height = tan(theta / 2);
	auto half_width = half_height * aspect_radio;

	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);

	origin = lookfrom;
	lower_left_corner = origin 
		- half_width * focus_dist * u 
		- half_height * focus_dist * v
		- focus_dist * w;

	lens_radius = aperture / 2.0;
	horization = 2.0 * half_width * focus_dist * u;
	vertical = 2.0 * half_height * focus_dist * v;
}

ray camera::get_ray(double s, double t) const {
	vec3 rd = lens_radius * random_in_unit_disk();
	vec3 offset = u * rd.x() + v * rd.y();

	return ray(origin + offset, lower_left_corner + s * horization + t * vertical - origin - offset);
}