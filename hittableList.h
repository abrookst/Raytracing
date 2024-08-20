#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "hittable.h"
#include "common.h"

class HittableList : public Hittable {
    public:
        std::vector<shared_ptr<Hittable>> objs;

        HittableList() {}
        HittableList(shared_ptr<Hittable> obj) { add(obj); } 

        void clear() { objs.clear(); }

        void add(shared_ptr<Hittable> obj){
            objs.push_back(obj);
        }

        bool hit(const Ray& r, float rayTMin, float rayTMax, HitRecord& rec)  const override {
            HitRecord tempRecord;
            bool hitAnything = false;
            double closest = rayTMax;

            for(const shared_ptr<Hittable>& obj : objs){
                if(obj->hit(r,rayTMin,closest,tempRecord)){
                    hitAnything = true;
                    closest = tempRecord.t;
                    rec = tempRecord;
                }
            }
            return hitAnything;
        }

};

#endif