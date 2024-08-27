#ifndef QUAD_H
#define QUAD_H

#include "common.h"

#include "hittable.h"
#include "hittablelist.h"

class Quad : public Hittable {
    public:
        Quad(const Point3& Q, const Vector3& u, const Vector3& v, shared_ptr<Material> mat) : Q(Q), u(u), v(v), mat(mat) {
            Vector3 n = cross(u,v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n,n);

            set_bounding_box();
        }

        virtual void set_bounding_box() {
            AABB bboxDiagonal1 = AABB(Q, Q + u + v);
            AABB bboxDiagonal2 = AABB(Q + u, Q + v);
            bbox = AABB(bboxDiagonal1, bboxDiagonal2);
        }

        AABB bounding_box() const override { return bbox; }

        bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
            float denom = dot(normal, r.direction());

            if (std::fabs(denom) < 1e-8){
                return false;
            }

            float t = (D - dot(normal, r.origin())) / denom;
            if (!rayT.contains(t)){
                return false;
            }

            Point3 intersection = r.at(t);
            Vector3 planar_hitpt_vector = intersection - Q;
            float alpha = dot(w, cross(planar_hitpt_vector, v));
            float beta = dot(w, cross(u, planar_hitpt_vector));

            if (!is_interior(alpha, beta, rec)){
                return false;
            }

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;
        }
    virtual bool is_interior(float a, float b, HitRecord& rec) const {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!unitInterval.contains(a) || !unitInterval.contains(b))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

    protected:
        Point3 Q;
        Vector3 u, v, w;
        shared_ptr<Material> mat;
        AABB bbox;
        Vector3 normal;
        float D;
};

class Triangle : public Quad {
    public:
    Triangle(const Point3& a, const Vector3& ab, const Vector3& ac, shared_ptr<Material> mat) : Quad(a, ab, ac, mat) {}

    virtual bool is_interior(float a, float b, HitRecord& rec) const override {
        if((a < 0) || (b < 0) || (a+b > 1)){
            return false;
        }

        rec.u = a;
        rec.v = b;
        return true;
    }
};

class Ellipse : public Quad {
  public:
    Ellipse(const Point3& center, const Vector3& sideA, const Vector3& sideB, shared_ptr<Material> mat) : Quad(center, sideA, sideB, mat){}

    virtual void set_bounding_box() override {
        bbox = AABB(Q - u - v, Q + u + v);
    }

    virtual bool is_interior(float a, float b, HitRecord& rec) const override {
        if ((a*a + b*b) > 1)
            return false;

        rec.u = a/2 + 0.5;
        rec.v = b/2 + 0.5;
        return true;
    }
};
#endif