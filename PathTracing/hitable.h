#ifndef S0008E_HITABLE_H
#define S0008E_HITABLE_H
#include "Vector4D.h"
#include "ray.h"
#include "aabb.h"
class material;
struct hitRecord
{
    float t;
    Vector4D p;
    float u;
    float v;
    Vector4D normal;
    material *matPtr;
};

class hitable
{
public:
    virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const = 0;
    virtual bool boundingBox(float t0, float t1, aabb& box) const = 0;
    virtual ~hitable(){}
    virtual float pdfValue(const Vector4D& o, const Vector4D& v) const {return 0.0f;}
    virtual Vector4D random(const Vector4D& o) const {return Vector4D(1,0,0,0);}
};
#endif //S0008E_HITABLE_H
