#ifndef S0008E_MATERIAL_H
#define S0008E_MATERIAL_H
#include "ray.h"
#include "hitable.h"
Vector4D randomInUnitSphere()
{
    Vector4D p;
    do
    {
        p = Vector4D(drand48(), drand48(), drand48(), 1)*2 - Vector4D(1, 1, 1, 1);
    }while(p.squaredLength() >= 1.0);
    return p;
}

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

class material
{
public:
    virtual bool scatter(const ray& r_in, const hitRecord& rec, Vector4D& attenuation, ray& scattered) const = 0;
    Vector4D reflect(const Vector4D& v, const Vector4D& n) const {return v - n*v.dotProduct(n)*2;}
};

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
#endif //S0008E_MATERIAL_H
