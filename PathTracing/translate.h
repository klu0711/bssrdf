#ifndef S0008E_TRANSLATE_H
#define S0008E_TRANSLATE_H

#include <float.h>
#include "hitable.h"
#include "Vector4D.h"

class translate : public hitable
{
public:
    translate(hitable* p, const Vector4D& disp) : ptr(p), offset(disp){}
    virtual bool hit(const ray& r, float t0, float t1, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb &box) const;

    hitable* ptr;
    Vector4D offset;
};

bool translate::hit(const ray &r, float t0, float t1, hitRecord& rec) const
{
    ray movedRay(r.origin() - offset, r.direction(), r.time());
    if(ptr->hit(movedRay, t0, t1, rec))
    {
        rec.p = rec.p + offset;
        return true;
    } else
        return false;
}

bool translate::boundingBox(float t0, float t1, aabb &box) const
{
    if(ptr->boundingBox(t0, t1, box))
    {
        box = aabb(box.min() + offset, box.max() + offset);
        return true;
    } else
        return false;
}

class rotateY : public hitable
{
public:
    rotateY(hitable* p, float angle);
    virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb &box) const
    {
        box = bbox;
        return hasbox;
    }
    hitable* ptr;
    float sinTheta;
    float cosTheta;
    bool hasbox;
    aabb bbox;
};

rotateY::rotateY(hitable *p, float angle) : ptr(p)
{
    float radians = (M_PI / 180.f) * angle;
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasbox = ptr->boundingBox(0, 1, bbox);
    Vector4D min(FLT_MAX, FLT_MAX, FLT_MAX, 1);
    Vector4D max(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                float x = i*bbox.max()[0] + (1-i)*bbox.min()[0];
                float y = j*bbox.max()[1] + (1-j)*bbox.min()[1];
                float z = k*bbox.max()[2] + (1-k)*bbox.min()[2];
                float newX = cosTheta*x + sinTheta*z;
                float newZ = -sinTheta*x + cosTheta*z;
                Vector4D tester(newX, y, newZ, 1);
                for (int c = 0; c < 3; ++c)
                {
                    if(tester[c] > max[c])
                        max[c] = tester[c];
                    if(tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
        }
    }
    bbox = aabb(min, max);

}

bool rotateY::hit(const ray &r, float tmin, float tmax, hitRecord &rec) const
{
    Vector4D origin = r.origin();
    Vector4D dir = r.direction();
    origin[0] = cosTheta*r.origin()[0] - sinTheta*r.origin()[2];
    origin[2] = sinTheta*r.origin()[0] + cosTheta*r.origin()[2];
    dir[0] = cosTheta*r.direction()[0] - sinTheta*r.direction()[2];
    dir[2] = sinTheta*r.direction()[0] + cosTheta*r.direction()[2];
    ray rotated(origin, dir, r.time());
    if(ptr->hit(rotated, tmin, tmax, rec))
    {
        Vector4D p = rec.p;
        Vector4D norm = rec.normal;
        p[0] = cosTheta*rec.p[0] + sinTheta*rec.p[2];
        p[2] = -sinTheta*rec.p[0] + cosTheta*rec.p[2];
        norm[0] = cosTheta*rec.normal[0] + sinTheta*rec.normal[2];
        norm[2] = -sinTheta*rec.normal[0] + cosTheta*rec.normal[2];
        rec.p = p;
        rec.normal = norm.normalize();
        assert(rec.normal.length() > 0.1f);
        return true;
    } else
        return false;
}

#endif //S0008E_TRANSLATE_H
