#ifndef S0008E_DIELECTRIC_H
#define S0008E_DIELECTRIC_H
#include "material.h"

bool refract(const Vector4D& v, const Vector4D& n, float niOverNt, Vector4D& refracted)
{
    Vector4D uv = v.normalize();
    float dt = uv.dotProduct(n);
    float discriminant = 1.0 - (niOverNt*niOverNt*(1-dt*dt));
    if(discriminant > 0)
    {
        refracted = (uv-n*dt)*niOverNt - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

float schlick(float cosine, float refIdx)
{
    float r0 = (1-refIdx) / (1+refIdx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1-cosine), 5);
}

class dielectric : public material
{
public:
    dielectric(float ri) : refIdx(ri) {}
    virtual bool scatter(const ray& rIn, const hitRecord& rec, Vector4D& attenuation, ray& scattered) const
    {
        Vector4D outwardNormal;
        Vector4D reflected = reflect(rIn.direction(), rec.normal);
        float niOverNt;
        attenuation = Vector4D(1.0, 1.0, 1.0, 1);
        Vector4D refracted;
        float reflectProb;
        float cosine;
        if(rIn.direction().dotProduct(rec.normal) > 0)
        {
            outwardNormal = rec.normal * -1;
            niOverNt = refIdx;
            cosine = refIdx * rIn.direction().dotProduct(rec.normal) / rIn.direction().length();
        }else
        {
            outwardNormal = rec.normal;
            niOverNt = 1.0 / refIdx;
            cosine = (rIn.direction().dotProduct(rec.normal)*-1)/rIn.direction().length();
        }if(refract(rIn.direction(), outwardNormal, niOverNt, refracted))
        {
            reflectProb = schlick(cosine, refIdx);

        }else
        {
            scattered = ray(rec.p, reflected);
            reflectProb = 1.0;
        }if(drand48() < reflectProb)
        {
            scattered = ray(rec.p, reflected);
        }else
        {
            scattered = ray(rec.p, refracted);
        }
        return true;

    }

    float refIdx;
};

#endif //S0008E_DIELECTRIC_H
