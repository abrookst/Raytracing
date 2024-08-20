#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class HitRecord {
    public: 
        Point3 p;
        Vector3 normal;
        float t;
        bool frontFace;

    void set_face_normal(const Ray& r, const Vector3& outwardNormal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
    public:
        virtual ~Hittable() = default;
        virtual bool hit(const Ray& r, float rayTMin, float rayTMax, HitRecord& rec) const = 0;
};

#endif