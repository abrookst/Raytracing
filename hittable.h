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
        float u;
        float v;
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

class Translate : public Hittable {
  public:
    Translate(shared_ptr<Hittable> object, const Vector3& offset) : object(object), offset(offset)
    {
        bbox = object->bounding_box() + offset;
    }

    AABB bounding_box() const override { return bbox; }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
        Ray offset_r(r.origin() - offset, r.direction(), r.time());

        if (!object->hit(offset_r, rayT, rec)){
            return false;
        }

        rec.p += offset;

        return true;
    }

  private:
    shared_ptr<Hittable> object;
    Vector3 offset;
    AABB bbox;
};


enum ROTATIONTYPE{
    X_ROTATION = 0,
    Y_ROTATION,
    Z_ROTATION,
};
  

class RotateX : public Hittable {
public:
RotateX(shared_ptr<Hittable> object, float angle) : object(object) {
    float radians = degrees_to_radians(angle);
    sinTheta = std::sin(radians);
    cosTheta = std::cos(radians);
    bbox = object->bounding_box();

    Point3 min( infinity,  infinity,  infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                float x = i*bbox.x.max + (1-i)*bbox.x.min;
                float y = j*bbox.y.max + (1-j)*bbox.y.min;
                float z = k*bbox.z.max + (1-k)*bbox.z.min;

                float newy = cosTheta*y + -sinTheta*z;
                float newz = sinTheta*y + cosTheta*z;

                Vector3 tester(x, newy, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = std::fmin(min[c], tester[c]);
                    max[c] = std::fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = AABB(min, max);
}

bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
    // Change the ray from world space to object space
    Vector3 origin = r.origin();
    Vector3 direction = r.direction();

    origin[1] =  cosTheta*r.origin()[1] + sinTheta*r.origin()[2];
    origin[2] = -sinTheta*r.origin()[1] + cosTheta*r.origin()[2];

    direction[1] =  cosTheta*r.direction()[1] + sinTheta*r.direction()[2];
    direction[2] = -sinTheta*r.direction()[1] + cosTheta*r.direction()[2];

    Ray rotated_r(origin, direction, r.time());

    // Determine whether an intersection exists in object space (and if so, where)
    if (!object->hit(rotated_r, rayT, rec))
        return false;

    // Change the intersection point from object space to world space
    Vector3 p = rec.p;
    p[1] = cosTheta*rec.p[1] - sinTheta*rec.p[2];
    p[2] = sinTheta*rec.p[1] + cosTheta*rec.p[2];

    // Change the normal from object space to world space
    Vector3 normal = rec.normal;
    normal[1] = cosTheta*rec.normal[1] - sinTheta*rec.normal[2];
    normal[2] = sinTheta*rec.normal[1] + cosTheta*rec.normal[2];

    rec.p = p;
    rec.normal = normal;

    return true;
}

AABB bounding_box() const override { return bbox; }

private:
shared_ptr<Hittable> object;
float sinTheta;
float cosTheta;
AABB bbox;
};

class RotateY : public Hittable {
public:
RotateY(shared_ptr<Hittable> object, float angle) : object(object) {
    float radians = degrees_to_radians(angle);
    sinTheta = std::sin(radians);
    cosTheta = std::cos(radians);
    bbox = object->bounding_box();

    Point3 min( infinity,  infinity,  infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                float x = i*bbox.x.max + (1-i)*bbox.x.min;
                float y = j*bbox.y.max + (1-j)*bbox.y.min;
                float z = k*bbox.z.max + (1-k)*bbox.z.min;

                float newx =  cosTheta*x + sinTheta*z;
                float newz = -sinTheta*x + cosTheta*z;

                Vector3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = std::fmin(min[c], tester[c]);
                    max[c] = std::fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = AABB(min, max);
}

bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
    // Change the ray from world space to object space
    Vector3 origin = r.origin();
    Vector3 direction = r.direction();

    origin[0] = cosTheta*r.origin()[0] - sinTheta*r.origin()[2];
    origin[2] = sinTheta*r.origin()[0] + cosTheta*r.origin()[2];

    direction[0] = cosTheta*r.direction()[0] - sinTheta*r.direction()[2];
    direction[2] = sinTheta*r.direction()[0] + cosTheta*r.direction()[2];

    Ray rotated_r(origin, direction, r.time());

    // Determine whether an intersection exists in object space (and if so, where)
    if (!object->hit(rotated_r, rayT, rec))
        return false;

    // Change the intersection point from object space to world space
    Vector3 p = rec.p;
    p[0] =  cosTheta*rec.p[0] + sinTheta*rec.p[2];
    p[2] = -sinTheta*rec.p[0] + cosTheta*rec.p[2];

    // Change the normal from object space to world space
    Vector3 normal = rec.normal;
    normal[0] =  cosTheta*rec.normal[0] + sinTheta*rec.normal[2];
    normal[2] = -sinTheta*rec.normal[0] + cosTheta*rec.normal[2];

    rec.p = p;
    rec.normal = normal;

    return true;
}

AABB bounding_box() const override { return bbox; }

private:
shared_ptr<Hittable> object;
float sinTheta;
float cosTheta;
AABB bbox;
};

class RotateZ : public Hittable {
public:
RotateZ(shared_ptr<Hittable> object, float angle) : object(object) {
    float radians = degrees_to_radians(angle);
    sinTheta = std::sin(radians);
    cosTheta = std::cos(radians);
    bbox = object->bounding_box();

    Point3 min( infinity,  infinity,  infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                float x = i*bbox.x.max + (1-i)*bbox.x.min;
                float y = j*bbox.y.max + (1-j)*bbox.y.min;
                float z = k*bbox.z.max + (1-k)*bbox.z.min;

                float newx = cosTheta*x + -sinTheta*y;
                float newy = sinTheta*x +  cosTheta*y;

                Vector3 tester(newx, newy, z);

                for (int c = 0; c < 3; c++) {
                    min[c] = std::fmin(min[c], tester[c]);
                    max[c] = std::fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = AABB(min, max);
}

bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
    // Change the ray from world space to object space
    Vector3 origin = r.origin();
    Vector3 direction = r.direction();

    origin[0] =  cosTheta*r.origin()[0] + sinTheta*r.origin()[1];
    origin[1] = -sinTheta*r.origin()[0] + cosTheta*r.origin()[1];

    direction[0] =  cosTheta*r.direction()[0] + sinTheta*r.direction()[1];
    direction[1] = -sinTheta*r.direction()[0] + cosTheta*r.direction()[1];

    Ray rotated_r(origin, direction, r.time());

    // Determine whether an intersection exists in object space (and if so, where)
    if (!object->hit(rotated_r, rayT, rec))
        return false;

    // Change the intersection point from object space to world space
    Vector3 p = rec.p;
    p[0] = cosTheta*rec.p[0] - sinTheta*rec.p[1];
    p[1] = sinTheta*rec.p[0] + cosTheta*rec.p[1];

    // Change the normal from object space to world space
    Vector3 normal = rec.normal;
    normal[0] = cosTheta*rec.normal[0] - sinTheta*rec.normal[1];
    normal[1] = sinTheta*rec.normal[0] + cosTheta*rec.normal[1];

    rec.p = p;
    rec.normal = normal;

    return true;
}

AABB bounding_box() const override { return bbox; }

private:
shared_ptr<Hittable> object;
float sinTheta;
float cosTheta;
AABB bbox;
};

shared_ptr<Hittable> rotate(shared_ptr<Hittable> object, float angleX, float angleY, float angleZ){
    if(angleX != 0){
        object = make_shared<RotateX>(object, angleX);
    }
    if(angleY != 0){
        object = make_shared<RotateY>(object, angleY);
    }
    if(angleZ != 0){
        object = make_shared<RotateZ>(object, angleZ);
    }
    return object;
}  

#endif