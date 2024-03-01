#pragma once

#include "hittable.h"

class Line : public hittable {
public:
	Line() {}
	Line(vec3 header, vec3 tail, shared_ptr<material> mat) : p1(header), p2(tail), mat_ptr(mat){}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

private:
	vec3 p1;
	vec3 p2;
	color col;
	shared_ptr<material> mat_ptr;

};