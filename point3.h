#ifndef POINT3_H
#define POINT3_H

#include <iostream>
#include "vector3.h"

class Point3 : public Vector3 {
    public:
        Point3(float vX, float vY, float vZ): Vector3(vX, vY, vZ) {}

};

#endif