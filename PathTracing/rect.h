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
        box = aabb(Vector4D(x0, y0, k-0.0001, 1),Vector4D(x1, y1, k+0.0001, 1));
        return true;
    }
    material* mp;
    float x0, x1, y0, y1, k;
};

class xzRect : public hitable
{
public:
    xzRect(){}
    xzRect(float _x0, float _x1, float _z0, float _z1, float _k, material* mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
    virtual bool hit(const ray& r, float t0, float t1, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb& box)const
    {
        box = aabb(Vector4D(x0, k-0.0001, z0  , 1),Vector4D(x1,k+0.0001, z1, 1));
        return true;
    }
    material* mp;
    float x0, x1, z0, z1, k;
};

class yzRect : public hitable
{
public:
    yzRect(){}
    yzRect(float _y0, float _y1, float _z0, float _z1, float _k, material* mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
    virtual bool hit(const ray& r, float t0, float t1, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb& box)const
    {
        box = aabb(Vector4D(k-0.0001, y0, z0, 1),Vector4D(k+0.0001, y1, z1, 1));
        return true;
    }
    material* mp;
    float y0, y1, z0, z1, k;
};

class flipNormal : public hitable
{
public:
    flipNormal(hitable* p) : ptr(p) {}
    virtual bool hit(const ray& r, float t0, float t1, hitRecord& rec) const
    {
        if(ptr->hit(r, t0, t1, rec))
        {
            rec.normal =  rec.normal * -1.f;
            return true;
        } else
            return false;
    }
    virtual bool boundingBox(float t0, float t1, aabb &box) const
    {
        return ptr->boundingBox(t0, t1, box);
    }
    hitable* ptr;
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

inline bool xzRect::hit(const ray &r, float t0, float t1, hitRecord &rec) const
{
    float t = (k-r.origin()[1]) / r.direction()[1];
    if(t < t0 || t > t1)
        return false;
    float x = r.origin()[0] + t*r.direction()[0];
    float z = r.origin()[2] + t*r.direction()[2];
    if(x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.matPtr = mp;
    rec.p = r.pointAtParameter(t);
    rec.normal = Vector4D(0,1,0,1);
    return true;
}

inline bool yzRect::hit(const ray &r, float t0, float t1, hitRecord &rec) const
{
    float t = (k-r.origin()[0]) / r.direction()[0];
    if(t < t0 || t > t1)
        return false;
    float y = r.origin()[1] + t*r.direction()[1];
    float z = r.origin()[2] + t*r.direction()[2];
    if(y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.matPtr = mp;
    rec.p = r.pointAtParameter(t);
    rec.normal = Vector4D(1,0,0,1);
    return true;
}

#endif //S0008E_RECT_H