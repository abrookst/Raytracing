#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "common.h"

#include "hittable.h"
#include "material.h"
#include "texture.h"

class ConstantMedium : public Hittable {
    public:
        ConstantMedium(shared_ptr<Hittable> boundary, float density, shared_ptr<Texture> tex) : boundary(boundary), negInvDensity(-1/density), phaseFunction(make_shared<Isotropic>(tex)){}

        ConstantMedium(shared_ptr<Hittable> boundary, float density, Color albedo) : boundary(boundary), negInvDensity(-1/density), phaseFunction(make_shared<Isotropic>(albedo)){}

        bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
            HitRecord rec1, rec2;

            if (!boundary->hit(r, Interval::universe, rec1))
                return false;

            if (!boundary->hit(r, Interval(rec1.t+0.0001, infinity), rec2))
                return false;

            if (rec1.t < rayT.min) rec1.t = rayT.min;
            if (rec2.t > rayT.max) rec2.t = rayT.max;

            if (rec1.t >= rec2.t)
                return false;

            if (rec1.t < 0)
                rec1.t = 0;

            float rayLength = r.direction().length();
            float distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
            float hit_distance = negInvDensity * std::log(random_float());

            if (hit_distance > distanceInsideBoundary)
                return false;

            rec.t = rec1.t + hit_distance / rayLength;
            rec.p = r.at(rec.t);

            rec.normal = Vector3(1,0,0);
            rec.frontFace = true;
            rec.mat = phaseFunction;

            return true;
        }

        AABB bounding_box() const override { return boundary->bounding_box(); }

    private:
        shared_ptr<Hittable> boundary;
        double negInvDensity;
        shared_ptr<Material> phaseFunction;
};

#endif