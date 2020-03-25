#ifndef S0008E_MATERIAL_H
#define S0008E_MATERIAL_H
#include "ray.h"
#include "hitable.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"
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

struct scatterRecord
{
    ray specularRay;
    bool isSpecular;
    Vector4D attenuation;
    pdf* pdfPtr;
};

class material
{
public:
    virtual bool scatter(const ray& r_in, const hitRecord& rec,scatterRecord& sRec) const { return false;}
    virtual float scatterPdf(const ray& r_in, const hitRecord& rec, const ray& scattered) const {return false;}
    Vector4D reflect(const Vector4D& v, const Vector4D& n) const {return v - n*v.dotProduct(n)*2;}
    virtual Vector4D emitted(const ray& r_in, const hitRecord& rec, float u,  float v, const Vector4D& p) const { return Vector4D(0,0,0,1);}
};



class mediumMaterial : public material
{
public:
    mediumMaterial(texture* a) : albedo(a) {}
    bool scatter(const ray& r_in, const hitRecord& rec, scatterRecord& sRec) const override
    {


        if(rec.insideMat)
        {
            ray scattered = ray(rec.p, randomInUnitSphere());
            sRec.attenuation = albedo->value(rec.u, rec.v, rec.p);
            sRec.specularRay = scattered;
            sRec.pdfPtr = 0;
            sRec.isSpecular = true;
            return true;
        } else
        {
            sRec.isSpecular = false;
            sRec.attenuation = albedo->value(rec.u, rec.v, rec.p);
            sRec.pdfPtr = new cosinePdf(rec.normal);
            return true;

        }

    }

    float scatterPdf(const ray& r_in, const hitRecord& rec, const ray& scattered) const override
    {
        Vector4D temp = scattered.direction().normalize();
        auto n = rec.normal;
        float cosine = n.dotProduct(temp);
        if(cosine < 0)
            cosine = 0;
        return cosine / RPI;
    }
    texture* albedo;
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

    bool scatter(const ray& r_in, const hitRecord& rec, scatterRecord& sRec) const
    {
        sRec.isSpecular = false;
        sRec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        sRec.pdfPtr = new cosinePdf(rec.normal);
        return true;
    }
    texture* albedo;
};


class metal : public material
{
public:
    metal(const Vector4D& a, float f) : albedo(a) {f<1 ? fuzz = f : fuzz = 1;};
    virtual bool scatter(const ray& r_in, const hitRecord& hRec, scatterRecord& sRec) const
    {
        Vector4D reflected = reflect(r_in.direction().normalize(), hRec.normal);
        sRec.specularRay = ray(hRec.p, reflected + randomInUnitSphere() * fuzz);
        sRec.attenuation = albedo;
        sRec.isSpecular = true;
        sRec.pdfPtr = 0;
        return true;
    }
    Vector4D albedo;
    float fuzz;
};
#endif //S0008E_MATERIAL_H
