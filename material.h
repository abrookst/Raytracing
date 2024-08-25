#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

#include "texture.h"

class HitRecord;

class Material {
    public:
        virtual ~Material() = default;

        virtual bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material{
    public:
        Lambertian(const Color& alb): tex(make_shared<SolidColor>(alb)) {}
        Lambertian(shared_ptr<Texture> tex) : tex(tex) {}

        bool scatter([[maybe_unused]] const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
            Vector3 scatterDirection = rec.normal + random_unit_vector();

            if (scatterDirection.near_zero()){
                scatterDirection = rec.normal;
            }
            

            scattered = Ray(rec.p, scatterDirection, rIn.time());
            attenuation = tex->value(rec.u, rec.v, rec.p);
            return true;
        }

    private:
        shared_ptr<Texture> tex;
};

class Metal : public Material{
    public:
        Metal(const Color& alb, float fuzz): albedo(alb), fuzz(fuzz) {}

        bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
            Vector3 reflected = reflect(rIn.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = Ray(rec.p, reflected, rIn.time());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);

        }

    private:
        Color albedo;
        float fuzz;

};

class Dielectric : public Material{
    public:
        Dielectric(const Color& alb, float refInd): albedo(alb), refractionIndex(refInd) {}

        bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
            attenuation = (Color(1.0, 1.0, 1.0) / 2) + (albedo / 2);
            float ri = rec.frontFace ? (1.0/refractionIndex) : refractionIndex;

            Vector3 unitDirection = unit_vector(rIn.direction());

            float cosTheta = std::fmin(dot(-unitDirection, rec.normal), 1.0);
            float sinTheta = std::sqrt(1.0 - cosTheta*cosTheta);

            bool cannotRefract = ri * sinTheta > 1.0;
            Vector3 direction;

            if (cannotRefract || reflectance(cosTheta, ri) > random_float()){
                direction = reflect(unitDirection, rec.normal);
            }    
            else{
                direction = refract(unitDirection, rec.normal, ri);
            }

            scattered = Ray(rec.p, direction, rIn.time());
            return true;

        }

    private:
        Color albedo;
        float refractionIndex;

        static float reflectance(float cosine, float refractionIndex) {
            // Use Schlick's approximation for reflectance.
            float r0 = (1 - refractionIndex) / (1 + refractionIndex);
            r0 = r0*r0;
            return r0 + (1-r0)*std::pow((1 - cosine),5);
        }

};

#endif