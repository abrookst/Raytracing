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
    float& operator[](int i) { return v[i]; }

    Vector3& operator+=(const Vector3 &vec)
    {
        v[0] += vec.v[0];
        v[1] += vec.v[1];
        v[2] += vec.v[2];
        return *this;
    }

    Vector3& operator*=(float t)
    {
        v[0] *= t;
        v[1] *= t;
        v[2] *= t;
        return *this;
    }

    Vector3& operator/=(float t)
    {
        return *this *= 1 / t;
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

double length_squared(const Vector3 &vec) {
    return vec.v[0]*vec.v[0] + vec.v[1]*vec.v[1] + vec.v[2]*vec.v[2];
}

double length(const Vector3 &vec) {
    return std::sqrt(length_squared(vec));
}

inline double dot(const Vector3& vec1, const Vector3& vec2) {
    return vec1[0] * vec2[0]
         + vec1[1] * vec2[1]
         + vec1[2] * vec2[2];
}

inline Vector3 cross(const Vector3& vec1, const Vector3& vec2) {
    return Vector3(vec1[1] * vec2[2] - vec1[2] * vec2[1],
                vec1[2] * vec2[0] - vec1[0] * vec2[2],
                vec1[0] * vec2[1] - vec1[1] * vec2[0]);
}

inline Vector3 operator/(const Vector3 &vec, float t)
{
    return (1 / t) * vec;
}

inline Vector3 unit_vector(const Vector3& vec) {
    return vec / length(vec);
}

#endif