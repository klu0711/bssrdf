#ifndef S0008E_HITABLELIST_H
#define S0008E_HITABLELIST_H
#include "hitable.h"
#include "aabb.h"
class hitableList : public hitable
{
public:
    hitableList() {}
    ~hitableList()
    {
        for (int i = 0; i < listSize; ++i)
        {
            delete list[i];
        }
        delete[] list;
    }
    hitableList(hitable **l, int n) {list = l; listSize = n;}
    virtual bool hit(const ray&, float tmin, float tmax, hitRecord& rec) const;
    bool boundingBox(float t0, float t1, aabb& box) const;
    hitable **list;
    int listSize;
};

bool hitableList::hit(const ray& r, float tmin, float tmax, hitRecord &rec) const
{
    hitRecord tempRec;
    bool hitAnything = false;
    double closestSoFar = tmax;
    for (int i = 0; i < listSize; ++i) {
        if(list[i]->hit(r, tmin, closestSoFar, tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }
    return hitAnything;
}

bool hitableList::boundingBox(float t0, float t1, aabb &box) const
{
    if(listSize < 1) return false;
    aabb tempBox;
    bool firstTrue = list[0]->boundingBox(t0, t1, tempBox);
    if(!firstTrue)
        return false;
    else
        box = tempBox;
    for (int i = 0; i < listSize; ++i)
    {
        if(list[0]->boundingBox(t0, t1, tempBox))
        {
            box = surroundingBox(box, tempBox);
        }
        else
            return false;
    }
    return true;
}
#endif //S0008E_HITABLELIST_H
