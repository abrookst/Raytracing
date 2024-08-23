#ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"

#include "aabb.h"

class Material;

class HitRecord {
    public: 
        Point3 p;
        Vector3 normal;
        shared_ptr<Material> mat;
        float t;
        bool frontFace;

    void set_face_normal(const Ray& r, const Vector3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
    public:
        virtual ~Hittable() = default;
        virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
        virtual AABB bounding_box() const = 0;
};

#endif