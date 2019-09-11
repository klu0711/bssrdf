#ifndef S0008E_HITABLE_H
#define S0008E_HITABLE_H
#include "Vector4D.h"
#include "ray.h"
struct hitRecord
{
    float t;
    Vector4D p;
    Vector4D normal;
};

class hitable
{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hitRecord& rec) const = 0;
};
#endif //S0008E_HITABLE_H
