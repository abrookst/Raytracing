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


inline shared_ptr<HittableList> Box(const Point3& a, const Point3& b, shared_ptr<Material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = make_shared<HittableList>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = Point3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
    auto max = Point3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

    auto dx = Vector3(max.x() - min.x(), 0, 0);
    auto dy = Vector3(0, max.y() - min.y(), 0);
    auto dz = Vector3(0, 0, max.z() - min.z());

    sides->add(make_shared<Quad>(Point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(make_shared<Quad>(Point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(make_shared<Quad>(Point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(make_shared<Quad>(Point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(make_shared<Quad>(Point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(make_shared<Quad>(Point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}

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