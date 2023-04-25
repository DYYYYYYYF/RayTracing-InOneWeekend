#include "sphere.h"

sphere::sphere(){}
sphere::sphere(point3 cen, double r) : center(cen), radius(r){}

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record &rec) const {

    vec3 oc = r.origin() - center;
    vec3 dir = r.direction();
    auto a = dir.length_squared();
    auto half_b = dot(oc, dir);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b -  a * c;
    if (discriminant < 0) return false;
   
    auto sqrted = std::sqrt(discriminant);

    auto root = (-half_b - sqrted) / a;
    if (root < t_min || t_max < root){
        root = (-half_b + sqrted) / a;
        if (root < t_min || t_max < root){
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
