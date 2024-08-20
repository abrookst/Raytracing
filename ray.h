#ifndef RAY_H
#define RAY_H

#include "vector3.h"
#include "point3.h"

class Ray {
public:
    Ray() : orig(Point3()), dir(Vector3()) {}

    Ray(const Point3 &origin, const Vector3 &direction) : orig(origin), dir(direction) {}

    Point3 at(float t) const {
        Point3 p = orig;
        p += t*dir;
        return p;
    }

    const Point3& origin() const  { return orig; }
    const Vector3& direction() const { return dir; }

private:
    Point3 orig;
    Vector3 dir;
};

#endif