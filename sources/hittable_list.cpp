#include "hittable_list.h"

hittable_list::hittable_list() {}
hittable_list::hittable_list(std::shared_ptr<hittable> object) {
	add(object);
}

hittable_list::~hittable_list() {}

void hittable_list::clear() {
	objects.clear();
}

void hittable_list::add(std::shared_ptr<hittable> object) {
	objects.push_back(object);
}

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;
	
	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {
			closest_so_far = temp_rec.t;
			hit_anything = true;
			rec = temp_rec;
		}
	}

	return hit_anything;
}
