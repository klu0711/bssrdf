#ifndef S0008E_HITABLE_H
#define S0008E_HITABLE_H
#include "Vector4D.h"
#include "ray.h"
class material;
struct hitRecord
{
    float t;
    Vector4D p;
    Vector4D normal;
    material *matPtr;
};

class hitable
{
public:
    virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const = 0;
   // ~hitable(){}
};
#endif //S0008E_HITABLE_H
