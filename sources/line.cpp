#include "line.h"

bool Line::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 dir = unit_vector(p1 - p2);
	double dis = (p1 - p2).length();
	vec3 l = p1 + dir * dis;

	double t = (l - r.origin()).length() / r.direction().length();
	if (t < t_min || t > t_max) return false;

	rec.p3 = r.origin() + t*r.direction();
	rec.front_face = true;
	rec.mat_ptr = mat_ptr;
	rec.normal = l;
	rec.t = t;

	return true;
}