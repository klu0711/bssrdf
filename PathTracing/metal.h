#ifndef S0008E_METAL_H
#define S0008E_METAL_H

#include "material.h"

class metal : public material
{
public:
    metal(const Vector4D& a, float f) : albedo(a) {f<1 ? fuzz = f : fuzz = 1;};
    virtual bool scatter(const ray& r_in, const hitRecord& rec, Vector4D& attenuation, ray& scattered) const
    {
        Vector4D reflected = reflect(r_in.direction().normalize(), rec.normal);
        scattered = ray(rec.p, reflected + randomInUnitSphere()*fuzz);
        attenuation = albedo;
        return (scattered.direction().dotProduct(rec.normal) > 0);
    }
    Vector4D albedo;
    float fuzz;
};

#endif //S0008E_METAL_H
