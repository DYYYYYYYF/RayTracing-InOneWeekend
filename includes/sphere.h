#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
	sphere();
	sphere(point3 center, double r, shared_ptr<material> mtl);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

#endif // !SPHERE_H
