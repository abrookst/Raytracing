#ifndef POINT3_H
#define POINT3_H

#include <iostream>
#include "vector3.h"

class Point3 : public Vector3 {
    public:
        Point3(): Vector3(0,0,0) {}
        Point3(float vX, float vY, float vZ): Vector3(vX, vY, vZ) {}
        Point3(Vector3 vec): Vector3(vec) {}

};

#endif