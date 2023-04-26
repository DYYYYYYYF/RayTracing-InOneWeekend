#include "hittable_list.h"
#include <memory>

hittable_list::hittable_list(){}
hittable_list::hittable_list(std::shared_ptr<hittable> object){
    add(object);
}

void hittable_list::add(std::shared_ptr<hittable> object){
    objects.push_back(object);
}

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto close_so_far = t_max;

    for (const auto& object: objects){
        if (object->hit(r, t_min, close_so_far, temp_rec)){
            hit_anything = true;
            close_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
