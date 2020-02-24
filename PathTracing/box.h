#ifndef S0008E_BOX_H
#define S0008E_BOX_H

#include "hitable.h"
#include "Vector4D.h"
#include "hitableList.h"
#include "rect.h"
class box : public hitable
{
public:
    box(){}
    box(const Vector4D& p0, const Vector4D& p1, material *ptr);
    virtual bool hit(const ray& r, float t0, float t1, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb &box) const
    {
        box = aabb(pmin, pmax);
        return true;
    }
    Vector4D pmin, pmax;
    hitable *listPtr;
};

box::box(const Vector4D &p0, const Vector4D &p1, material *ptr)
{
    pmin = p0;
    pmax = p1;
    hitable** list = new hitable*[6];
    list[0] = new flipNormal(new xyRect(p0[0], p1[0], p0[1], p1[1], p1[2], ptr));
    list[1] = new xyRect(p0[0], p1[0], p0[1], p1[1], p0[2], ptr);
    list[2] = new flipNormal(new xzRect(p0[0], p1[0], p0[2], p1[2], p1[1], ptr));
    list[3] = new xzRect(p0[0], p1[0], p0[2], p1[2], p0[1], ptr);
    list[4] = new flipNormal(new yzRect(p0[1], p1[1], p0[2], p1[2], p1[0], ptr));
    list[5] = new yzRect(p0[1], p1[1], p0[2], p1[2], p0[0], ptr);
    listPtr = new hitableList(list, 6);

}

bool box::hit(const ray &r, float t0, float t1, hitRecord &rec) const
{
    return listPtr->hit(r, t0, t1, rec);
}

#endif //S0008E_BOX_H
