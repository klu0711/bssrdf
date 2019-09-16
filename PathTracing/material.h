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
class material
{
public:
    virtual bool scatter(const ray& r_in, const hitRecord& rec, Vector4D& attenuation, ray& scattered) const = 0;
    Vector4D reflect(const Vector4D& v, const Vector4D& n) const {return v - n*v.dotProduct(n)*2;}
};

#endif //S0008E_MATERIAL_H
