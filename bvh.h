#ifndef BVH_H
#define BVH_H

#include "common.h"

#include "aabb.h"
#include "hittable.h"
#include "hittableList.h"

#include <algorithm>

class BVHNode : public Hittable
{
public:
    BVHNode(HittableList list) : BVHNode(list.objs, 0, list.objs.size()) {}

    BVHNode(std::vector<shared_ptr<Hittable>> &objs, size_t start, size_t end)
    {
        bbox = AABB::empty;
        for (size_t objectIndex=start; objectIndex < end; objectIndex++)
            bbox = AABB(bbox, objs[objectIndex]->bounding_box());

        int axis = bbox.longest_axis();

        auto comparator = (axis == 0)   ? box_x_compare
                          : (axis == 1) ? box_y_compare
                                        : box_z_compare;

        size_t objectSpan = end - start;

        if (objectSpan == 1)
        {
            left = right = objs[start];
        }
        else if (objectSpan == 2)
        {
            left = objs[start];
            right = objs[start + 1];
        }
        else
        {
            std::sort(std::begin(objs) + start, std::begin(objs) + end, comparator);

            size_t mid = start + objectSpan / 2;
            left = make_shared<BVHNode>(objs, start, mid);
            right = make_shared<BVHNode>(objs, mid, end);
        }
    }

    bool hit(const Ray &r, Interval rayT, HitRecord &rec) const override
    {
        if (!bbox.hit(r, rayT))
        {
            return false;
        }

        bool hitLeft = left->hit(r, rayT, rec);
        bool hitRight = right->hit(r, Interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);
        return hitLeft || hitRight;
    }

    AABB bounding_box() const override { return bbox; }

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB bbox;

    static bool box_compare(
        const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis_index
    ) {
        Interval aAxisInterval = a->bounding_box().axis_interval(axis_index);
        Interval bAxisInterval = b->bounding_box().axis_interval(axis_index);
        return aAxisInterval.min < bAxisInterval.min;
    }

    static bool box_x_compare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare (const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
        return box_compare(a, b, 2);
    }
};

#endif