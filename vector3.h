#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3
{
public:
    float v[3];

    Vector3() : v{0, 0, 0} {}
    Vector3(float vX, float vY, float vZ) : v{vX, vY, vZ} {}

    float x() const { return v[0]; }
    float y() const { return v[1]; }
    float z() const { return v[2]; }

    Vector3 operator-() const { return Vector3(-v[0], -v[1], -v[2]); }
    float operator[](int i) const { return v[i]; }
    float &operator[](int i) { return v[i]; }

    Vector3 &operator+=(const Vector3 &vec)
    {
        v[0] += vec.v[0];
        v[1] += vec.v[1];
        v[2] += vec.v[2];
        return *this;
    }

    Vector3 &operator*=(float t)
    {
        v[0] *= t;
        v[1] *= t;
        v[2] *= t;
        return *this;
    }

    Vector3 &operator/=(float t)
    {
        return *this *= 1 / t;
    }

    float length_squared() const
    {
        return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    }

    float length() const
    {
        return std::sqrt(length_squared());
    }

    static Vector3 random()
    {
        return Vector3(random_float(), random_float(), random_float());
    }

    static Vector3 random(float min, float max)
    {
        return Vector3(random_float(min, max), random_float(min, max), random_float(min, max));
    }

    bool near_zero() const
    {
        float s = 1e-8;
        return (std::fabs(v[0]) < s) && (std::fabs(v[1]) < s) && (std::fabs(v[2]) < s);
    }
};

// Vector Utility Functions

inline std::ostream &operator<<(std::ostream &out, const Vector3 &vec)
{
    return out << vec.v[0] << ' ' << vec.v[1] << ' ' << vec.v[2];
}

inline Vector3 operator+(const Vector3 &vec1, const Vector3 &vec2)
{
    return Vector3(vec1.v[0] + vec2.v[0], vec1.v[1] + vec2.v[1], vec1.v[2] + vec2.v[2]);
}

inline Vector3 operator-(const Vector3 &vec1, const Vector3 &vec2)
{
    return Vector3(vec1.v[0] - vec2.v[0], vec1.v[1] - vec2.v[1], vec1.v[2] - vec2.v[2]);
}

inline Vector3 operator*(const Vector3 &vec1, const Vector3 &vec2)
{
    return Vector3(vec1.v[0] * vec2.v[0], vec1.v[1] * vec2.v[1], vec1.v[2] * vec2.v[2]);
}

inline Vector3 operator*(float t, const Vector3 &vec)
{
    return Vector3(t * vec.v[0], t * vec.v[1], t * vec.v[2]);
}

inline Vector3 operator*(const Vector3 &vec, float t)
{
    return t * vec;
}

inline float dot(const Vector3 &vec1, const Vector3 &vec2)
{
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

inline Vector3 cross(const Vector3 &vec1, const Vector3 &vec2)
{
    return Vector3(vec1[1] * vec2[2] - vec1[2] * vec2[1],
                   vec1[2] * vec2[0] - vec1[0] * vec2[2],
                   vec1[0] * vec2[1] - vec1[1] * vec2[0]);
}

inline Vector3 operator/(const Vector3 &vec, float t)
{
    return (1 / t) * vec;
}

inline Vector3 unit_vector(const Vector3 &vec)
{
    return vec / vec.length();
}

inline Vector3 random_in_unit_disk() {
    while (true) {
        Vector3 p = Vector3(random_float(-1,1), random_float(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

inline Vector3 random_in_unit_sphere()
{
    while (true)
    {
        Vector3 p = Vector3::random(-1, 1);
        if (p.length_squared() < 1)
            return p;
    }
}

inline Vector3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

inline Vector3 random_on_hemisphere(const Vector3 &normal)
{
    Vector3 onUnitSphere = random_unit_vector();
    if (dot(onUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
        return onUnitSphere;
    else
        return -onUnitSphere;
}

inline Vector3 reflect(const Vector3 &v, const Vector3 &n)
{
    return v - 2 * dot(v, n) * n;
}

inline Vector3 refract(const Vector3& uv, const Vector3& n, float etaiOverEtat) {
    float cosTheta = std::fmin(dot(-uv, n), 1.0);
    Vector3 rOutPerp =  etaiOverEtat * (uv + cosTheta*n);
    Vector3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.length_squared())) * n;
    return rOutPerp + rOutParallel;
}

#endif