#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "common.h"
#include "hittable.h"

class HittableList : public Hittable {
    public:
        std::vector<shared_ptr<Hittable>> objs;

        HittableList() {}
        HittableList(shared_ptr<Hittable> obj) { add(obj); } 

        void clear() { objs.clear(); }

        AABB bounding_box() const override { return bbox; }

        void add(shared_ptr<Hittable> obj){
            objs.push_back(obj);
            bbox = AABB(bbox, obj->bounding_box());
        }

        bool hit(const Ray& r, Interval rayT, HitRecord& rec)  const override {
            HitRecord tempRecord;
            bool hitAnything = false;
            float closest = rayT.max;

            for(const shared_ptr<Hittable>& obj : objs){
                if(obj->hit(r,Interval(rayT.min,closest),tempRecord)){
                    hitAnything = true;
                    closest = tempRecord.t;
                    rec = tempRecord;
                }
            }
            return hitAnything;
        }
        private:
        AABB bbox;

};

#endif