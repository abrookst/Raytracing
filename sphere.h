#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "common.h"

class Sphere : public Hittable
{
public:
    // Stationary Sphere
    Sphere(const Point3& center, float radius, shared_ptr<Material> mat)
      : cen1(center), rad(std::fmax(0,radius)), mat(mat), isMoving(false) {
        Vector3 radVec = Vector3(radius,radius,radius);
        bbox = AABB(center - radVec, center + radVec);
      }

    // Moving Sphere
    Sphere(const Point3& center1, const Point3& center2, float radius,
           shared_ptr<Material> mat)
      : cen1(center1), rad(std::fmax(0,radius)), mat(mat), isMoving(true)
    {
        Vector3 radVec = Vector3(radius,radius,radius);
        AABB bbox1(center1 - radVec, center1 + radVec);
        AABB bbox2(center2 - radVec, center2 + radVec);
        bbox = AABB(bbox1, bbox2);

        centerVec = center2 - center1;
    }

    AABB bounding_box() const override { return bbox; }

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
        get_sphere_uv(outwardNormal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

private:
    Point3 cen1;
    float rad;
    shared_ptr<Material> mat;
    bool isMoving;
    Vector3 centerVec;
    AABB bbox;

    Point3 sphere_center(float time) const {
        // Linearly interpolate from center1 to center2 according to time, where t=0 yields
        // center1, and t=1 yields center2.
        return cen1 + time*centerVec;
    }

    static void get_sphere_uv(const Point3& p, float& u, float& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        float theta = std::acos(-p.y());
        float phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};

#endif