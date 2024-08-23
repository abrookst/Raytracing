#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "common.h"

class Sphere : public Hittable
{
public:
    // Stationary Sphere
    Sphere(const Point3& center, double radius, shared_ptr<Material> mat)
      : cen1(center), rad(std::fmax(0,radius)), mat(mat), isMoving(false) {}

    // Moving Sphere
    Sphere(const Point3& center1, const Point3& center2, double radius,
           shared_ptr<Material> mat)
      : cen1(center1), rad(std::fmax(0,radius)), mat(mat), isMoving(true)
    {
        centerVec = center2 - center1;
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override
    {
        Point3 cen = isMoving ? sphere_center(r.time()) : cen1;
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
        if (!rayT.surrounds(root))
        {
            root = (h + sqrtd) / a;
            if (!rayT.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - cen) / rad;
        Vector3 outwardNormal = (rec.p - cen) / rad;
        rec.set_face_normal(r, outwardNormal);
        rec.mat = mat;

        return true;
    }

private:
    Point3 cen1;
    float rad;
    shared_ptr<Material> mat;
    bool isMoving;
    Vector3 centerVec;

    Point3 sphere_center(double time) const {
        // Linearly interpolate from center1 to center2 according to time, where t=0 yields
        // center1, and t=1 yields center2.
        return cen1 + time*centerVec;
    }
};

#endif