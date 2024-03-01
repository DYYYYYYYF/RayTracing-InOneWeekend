#include "sphere.h"

sphere::sphere() {}
sphere::sphere(point3 center, double r, shared_ptr<material> mtl) : center(center), radius(r), mat_ptr(mtl){}

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c =  oc.length_squared() - radius * radius;

	auto disciminant = half_b * half_b - a * c;
	if (disciminant < 0) {
		return false;
	}

	auto t = (-half_b - std::sqrt(disciminant)) / a;
	if (t < t_min || t > t_max) {
		t = (-half_b + std::sqrt(disciminant) / a);
		if (t < t_min || t > t_max) {
			return false;
		}
	}

	rec.t = t;
	rec.p3 = r.at(t);
	vec3 outward_normal = (rec.p3 - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}