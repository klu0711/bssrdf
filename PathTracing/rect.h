#ifndef S0008E_RECT_H
#define S0008E_RECT_H

#include "material.h"
#include "hitable.h"
#include "aabb.h"
#include "Vector4D.h"
class xyRect : public hitable
{
public:
    xyRect(){}
    xyRect(float _x0, float _x1, float _y0, float _y1, float _k, material* mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
    virtual bool hit(const ray& r, float t0, float t1, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb& box)const
    {
        box = aabb(Vector4D(x0, y0, k-0.0001, 1),Vector4D(x0, y1, k+0.0001, 1));
        return true;
    }
    material* mp;
    float x0, x1, y0, y1, k;
};

inline bool xyRect::hit(const ray &r, float t0, float t1, hitRecord &rec) const
{
    float t = (k-r.origin()[2]) / r.direction()[2];
    if(t < t0 || t > t1)
        return false;
    float x = r.origin()[0] + t*r.direction()[0];
    float y = r.origin()[1] + t*r.direction()[1];
    if(x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    rec.matPtr = mp;
    rec.p = r.pointAtParameter(t);
    rec.normal = Vector4D(0,0,1,1);
    return true;
}

#endif //S0008E_RECT_H