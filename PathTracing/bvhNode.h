#ifndef S0008E_BVHNODE_H
#define S0008E_BVHNODE_H
#include "hitable.h"
#include boundingBox

class bvhNode : public hitable
{
public:
    bvhNode(){}
    bvhNode(hitable **l, int n, float time0, float time1);
    virtual bool hit(const ray& r, float tmin, float tmax, hitRecord & rec) const;
    virtual bool boundingBox(float t0, float t1, aabb& box) const;
    hitable *left;
    hitable *right;
    aabb box;


};

bool bvhNode::boundingBox(float t0, float t1, aabb& box) const
{
    b = box;
    return true;
}


#endif //S0008E_BVHNODE_H