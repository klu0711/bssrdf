#ifndef S0008E_DIFFUSELIGHT_H
#define S0008E_DIFFUSELIGHT_H

#include "material.h"
#include "texture.h"
#include "ray.h"

class diffuseLight : public material
{
public:
    diffuseLight(texture* a) : emit(a){}
    virtual bool scatter(const ray& r_in, const hitRecord& rec, Vector4D& attenuation, ray& scattered) const {return false;}
    virtual Vector4D emitted(const ray& r_in, const hitRecord& rec, float u, float v, const Vector4D& p) const
    {
        if(rec.normal.dotProduct(r_in.direction()) < 0.0f)
        {
            return emit->value(u, v, p);
        } else
        {
            return Vector4D(0,0,0,0);
        }
    }

    texture* emit;
};



#endif //S0008E_DIFFUSELIGHT_H