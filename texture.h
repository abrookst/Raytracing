#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

class Texture {
    public:
        virtual ~Texture() =  default;

        virtual Color value(float u, float v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor(const Color& albedo) : albedo(albedo) {}

        SolidColor(float r, float g, float b) : SolidColor (Color(r,g,b)) {}

        Color value([[maybe_unused]]float u, [[maybe_unused]]float v, [[maybe_unused]]const Point3& p) const override {
            return albedo;
        }
    private:
        Color albedo;
};

class CheckerTexture : public Texture {
    public:
    CheckerTexture(float scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) : invScale(1.0f / scale), even(even), odd(odd) {}

    CheckerTexture(float scale, const Color& c1, const Color& c2) : CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

    Color value(float u, float v, const Point3& p) const override {
        int xInteger = int(std::floor(invScale * p.x()));
        int yInteger = int(std::floor(invScale * p.y()));
        int zInteger = int(std::floor(invScale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }


    private:
    float invScale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

#endif