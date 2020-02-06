#ifndef S0008E_NODE_H
#define S0008E_NODE_H

#include <iostream>
#include "hitable.h"
#include <stdlib.h>

int boxXCompare(const void * a, const void * b)
{
    aabb boxLeft, boxRight;
    hitable* ah = *(hitable**)a;
    hitable* bh = *(hitable**)b;
    if(!ah->boundingBox(0,0,boxLeft) || !bh->boundingBox(0,0, boxRight))
        std::cerr << "error" << std::endl;
    if(boxLeft.min()[0] - boxRight.min()[0] < 0.0f)
        return -1;
    else
        return 1;
}

int boxYCompare(const void * a, const void * b)
{
    aabb boxLeft, boxRight;
    hitable* ah = *(hitable**)a;
    hitable* bh = *(hitable**)b;
    if(!ah->boundingBox(0,0,boxLeft) || !bh->boundingBox(0,0, boxRight))
        std::cerr << "error" << std::endl;
    if(boxLeft.min()[1] - boxRight.min()[1] < 0.0f)
        return -1;
    else
        return 1;
}

int boxZCompare(const void * a, const void * b)
{
    aabb boxLeft, boxRight;
    hitable* ah = *(hitable**)a;
    hitable* bh = *(hitable**)b;
    if(!ah->boundingBox(0,0,boxLeft) || !bh->boundingBox(0,0, boxRight))
        std::cerr << "error" << std::endl;
    if(boxLeft.min()[2] - boxRight.min()[2] < 0.0f)
        return -1;
    else
        return 1;
}

class bvhNode :  public hitable
{
public:
    bvhNode(){}
    bvhNode(hitable **l, int n, float time0, float time1);
    virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb& box) const;
    hitable *left;
    hitable *right;
    aabb box;
};

bvhNode::bvhNode(hitable **l, int n, float time0, float time1)
{
    int axis = int(3*drand48());
    if(axis == 0)
        qsort(l, n, sizeof(hitable*), boxXCompare);
    else if(axis == 1)
        qsort(l, n, sizeof(hitable*), boxYCompare);
    else if(axis == 2)
        qsort(l, n, sizeof(hitable*), boxZCompare);
    if(n == 1)
        left = right = l[0];
    else if(n == 2)
    {
        left = l[0];
        right = l[1];
    }
    else
    {
        left = new bvhNode(l, n/2, time0, time1);
        right = new bvhNode(l + n/2, n - n/2, time0, time1);
    }
    aabb boxLeft, boxRight;
    if(!left->boundingBox(time0, time1, boxLeft) || !right->boundingBox(time0, time1, boxRight))
        std::cerr << "Error" << std::endl;
    box = surroundingBox(boxLeft, boxRight);
}

bool bvhNode::boundingBox(float t0, float t1, aabb &b) const
{
    b = box;
    return true;
}

bool bvhNode::hit(const ray &r, float tmin, float tmax, hitRecord &rec) const
{
    if(box.hit(r, tmin, tmax))
    {
        hitRecord leftRec, rightRec;
        bool hitLeft = left->hit(r, tmin, tmax, leftRec);
        bool hitRight = right->hit(r, tmin, tmax, rightRec);
        if(hitLeft && hitRight)
        {
            if(leftRec.t < rightRec.t)
                rec = leftRec;
            else
                rec = rightRec;
            return true;
        }
        else if(hitLeft)
        {
            rec = leftRec;
            return true;
        }
        else if(hitRight)
        {
            rec = rightRec;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}
#endif //S0008E_NODE_H
