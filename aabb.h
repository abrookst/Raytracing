#ifndef AABB_H
#define AABB_H

#include "common.h"

class AABB
{
public:
    Interval x, y, z;

    AABB() {}
    AABB(const Interval &x, const Interval &y, const Interval &z) : x(x), y(y), z(z) {}
    AABB(const Point3 &a, const Point3 &b)
    {
        x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
    }
    AABB(const AABB& box0, const AABB& box1) {
        x = Interval(box0.x, box1.x);
        y = Interval(box0.y, box1.y);
        z = Interval(box0.z, box1.z);
    }

    const Interval &axis_interval(uint16_t n) const
    {
        if (n == 1)
        {
            return y;
        }
        if (n == 2)
        {
            return z;
        }
        return x;
    }

    bool hit(const Ray &r, Interval rayT) const
    {
        const Point3 &rayOrig = r.origin();
        const Vector3 &rayDir = r.direction();

        for (int axis = 0; axis < 3; axis++)
        {
            const Interval& ax = axis_interval(axis);
            const float adinv = 1.0 / rayDir[axis];

            float t0 = (ax.min - rayOrig[axis]) * adinv;
            float t1 = (ax.max - rayOrig[axis]) * adinv;

            if (t0 < t1)
            {
                if (t0 > rayT.min)
                    rayT.min = t0;
                if (t1 < rayT.max)
                    rayT.max = t1;
            }
            else
            {
                if (t1 > rayT.min)
                    rayT.min = t1;
                if (t0 < rayT.max)
                    rayT.max = t0;
            }

            if (rayT.max <= rayT.min)
                return false;
        }
        return true;
    }

    int longest_axis() const {
        // Returns the index of the longest axis of the bounding box.

        if (x.size() > y.size())
            return x.size() > z.size() ? 0 : 2;
        else
            return y.size() > z.size() ? 1 : 2;
    }

    static const AABB empty, universe;
};

const AABB AABB::empty    = AABB(Interval::empty,    Interval::empty,    Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

#endif