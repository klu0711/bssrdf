#ifndef S0008E_AABB_H
#define S0008E_AABB_H

#include <algorithm>
#include "Vector4D.h"
#include "ray.h"

inline float ffmin(float a, float b){return a < b ? a : b;}
inline float ffmax(float a, float b){return a > b ? a : b;}

class aabb
{
public:
    aabb();
    aabb(const Vector4D& a, const Vector4D& b) {_min = a; _max = b;}

    Vector4D min() const {return _min;}
    Vector4D max() const {return _max;}

    bool hit(const ray& r, float tmin, float tmax) const;
    Vector4D _min, _max;
};

inline bool aabb::hit(const ray& r, float tmin, float tmax) const
{
    for (int a = 0; a < 3; a++)
    {
        float invD = 1.0f / r.direction()[a];
        float t0 = ((min()[a] - r.origin()[a]) * invD);
        float t1 = ((max()[a] - r.origin()[a]) * invD);
        if(invD < 0.0f)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;

        if(tmax <= tmin)
            return false;
    }
    return true;
}

#endif //S0008E_AABB_H