#ifndef S0008E_HITABLELIST_H
#define S0008E_HITABLELIST_H
#include "hitable.h"
class hitableList : public hitable
{
public:
    hitableList() {}
    //~hitableList(){delete list;}
    hitableList(hitable **l, int n) {list = l; listSize = n;}
    virtual bool hit(const ray&, float tmin, float tmax, hitRecord& rec) const;
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
#endif //S0008E_HITABLELIST_H
