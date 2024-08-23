#ifndef RAY_H
#define RAY_H

#include "vector3.h"
#include "point3.h"

class Ray {
public:
    Ray() {}

    Ray(const Point3& origin, const Vector3& direction, float time)
      : orig(origin), dir(direction), tm(time) {}

    Ray(const Point3& origin, const Vector3& direction)
      : Ray(origin, direction, 0) {}

    const Point3& origin() const  { return orig; }
    const Vector3& direction() const { return dir; }
    const float& time() const { return tm; }

    Point3 at(float t) const {
        Point3 p = orig;
        p += t*dir;
        return p;
    }

private:
    Point3 orig;
    Vector3 dir;
    float tm;
};

#endif