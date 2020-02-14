#ifndef S0008E_DIFFUSELIGHT_H
#define S0008E_DIFFUSELIGHT_H

#include "material.h"
#include "texture.h"
#include "ray.h"

class diffuseLight : public material
{
public:
    diffuseLight(texture* a) : emit(a){}
    virtual bool scatter(const ray& r_in, const hit& rec, Vector4D& attenuation, ray& scattered) {return false;}
    virtual Vector4D emitted(float u, flaot v, const Vector4D& p) const {return emit->value(u, v, p);}

    texture* emit;
};

#endif //S0008E_DIFFUSELIGHT_H