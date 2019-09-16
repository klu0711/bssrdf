#ifndef S0008E_LAMBERTIAN_H
#define S0008E_LAMBERTIAN_H

#include "material.h"

class lambertian : public material
{
public:
    lambertian(const Vector4D& a): albedo(a) {}
    virtual bool scatter(const ray& r_in, const hitRecord& rec, Vector4D& attenuation, ray& scattered) const {
        Vector4D target = rec.p + rec.normal + randomInUnitSphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
    Vector4D albedo;
};

#endif //S0008E_LAMBERTIAN_H
