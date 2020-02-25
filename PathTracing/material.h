#ifndef S0008E_MATERIAL_H
#define S0008E_MATERIAL_H
#include "ray.h"
#include "hitable.h"
#include "texture.h"
#include "onb.h"

uint64_t s[2] = {0,1};

const float RPI = 3.141592;

float xorShift(void)
{
    static unsigned int x = 123456789;
    static unsigned int y = 362436069;
    static unsigned int z = 521288629;
    static unsigned int w = 88675123;
    unsigned int t;
    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    w = w ^ (w >> 19) ^ (t ^ (t >> 8));

    return static_cast<float>(w) / static_cast<float>(0xffffffff);
}

Vector4D randomCosineDir()
{
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrt(1-r2);
    float phi = 2*M_PI*r1;
    float x = cos(phi)*2*sqrt(r2);
    float y = sin(phi)*2*sqrt(r2);
    return Vector4D(x, y, z, 1);
}

Vector4D randomInUnitSphere()
{
    Vector4D p;
    do
    {
        p = Vector4D(xorShift(), xorShift(), xorShift(), 1)*2 - Vector4D(1, 1, 1, 1);
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
    virtual bool scatter(const ray& r_in, const hitRecord& rec, Vector4D& albedo, ray& scattered, float&pdf) const { return false;}
    virtual float scatterPdf(const ray& r_in, const hitRecord& rec, const ray& scattered) const {return false;}
    Vector4D reflect(const Vector4D& v, const Vector4D& n) const {return v - n*v.dotProduct(n)*2;}
    virtual Vector4D emitted(float u,  float v, const Vector4D& p) const { return Vector4D(0,0,0,1);}
};

class lambertian : public material
{
public:
    lambertian(texture* a): albedo(a) {}

    float scatterPdf(const ray& r_in, const hitRecord& rec, const ray& scattered) const override
    {
        Vector4D temp = scattered.direction().normalize();
        auto n = rec.normal;
        float cosine = n.dotProduct(temp);
        if(cosine < 0)
            cosine = 0;
        return cosine / RPI;
    }

    bool scatter(const ray& r_in, const hitRecord& rec, Vector4D& alb, ray& scattered, float& pdf) const
    {
        //Vector4D target = rec.p + rec.normal + randomInUnitSphere();
        //scattered = ray(rec.p, (target-rec.p).normalize(), r_in.time());
        //alb = albedo->value(rec.u, rec.v, rec.p);
        //pdf = rec.normal.dotProduct(scattered.direction()) / RPI;
        //return true;
        onb uvw;
        uvw.buildFromW(rec.normal);
        Vector4D dir = uvw.local(randomCosineDir());
        scattered = ray(rec.p, dir.normalize(), r_in.time());
        alb = albedo->value(rec.u, rec.v, rec.p);
        pdf = (uvw.w().dotProduct(scattered.direction())) / RPI;
        return true;
    }
    texture* albedo;
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
