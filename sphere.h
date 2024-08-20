#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vector3.h"

class Sphere : public Hittable
{
public:
    Sphere(const Point3 &center, float radius) : cen(center), rad(std::fmax(0, radius)) {}

    bool hit(const Ray &r, float rayTMin, float rayTMax, HitRecord &rec) const override
    {
        Vector3 oc = cen - r.origin();
        float a = r.direction().length_squared();
        float h = dot(r.direction(), oc);
        float c = oc.length_squared() - rad * rad;

        float discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        float sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        float root = (h - sqrtd) / a;
        if (root <= rayTMin || rayTMax <= root)
        {
            root = (h + sqrtd) / a;
            if (root <= rayTMin || rayTMax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - cen) / rad;
        Vector3 outwardNormal = (rec.p - cen) / rad;
        rec.set_face_normal(r, outwardNormal);

        return true;
    }

private:
    Point3 cen;
    float rad;
};

#endif