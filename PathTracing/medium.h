#ifndef S0008E_MEDIUM_H
#define S0008E_MEDIUM_H

#include <cfloat>
#include <cassert>
#include "hitable.h"
#include "texture.h"
#include "material.h"

class medium : public hitable
{
public:
    medium(hitable* h, float d, texture *a = 0) : boundary(h), density(d) {phaseFunction = new mediumMaterial(a);}

    virtual bool hit(const ray& r_in, float tmin, float tmax, hitRecord& rec) const;

    virtual bool boundingBox(float t0, float t1, aabb &box) const
    {
        return boundary->boundingBox(t0, t1, box);
    }

    hitable* boundary;
    float density;
    material* phaseFunction;
};

bool medium::hit(const ray &r_in, float tmin, float tmax, hitRecord &rec) const
{
    bool db =  (drand48() < 0.00001);
    db = false;
    hitRecord rec1, rec2;
    if(boundary->hit(r_in, -FLT_MAX, FLT_MAX, rec1))
    {
        if(boundary->hit(r_in, rec1.t + 0.001, FLT_MAX, rec2))
        {
    //if(db) assert(0);
            if(rec1.t < tmin)
                rec1.t = tmin;
            if(rec2.t > tmax)
                rec2.t = tmax;
            if(rec1.t >= rec2.t)
                return false;
            if(rec1.t < 0)
                rec1.t = 0;

            float distanceInsideBounds = (rec2.t - rec1.t)*r_in.direction().length();
            float hitDistance = -(1/density)*log(drand48());
            if(hitDistance < distanceInsideBounds)
            {
                rec.t = rec1.t + hitDistance / r_in.direction().length();
                rec.p = r_in.pointAtParameter(rec.t);
                rec.normal = Vector4D(1,0,0,0);
                rec.matPtr = phaseFunction;
                return true;
            }


        }
    }
    return false;
}

#endif //S0008E_MEDIUM_H
