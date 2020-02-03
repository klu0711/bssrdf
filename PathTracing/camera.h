#ifndef S0008E_CAMERA_H
#define S0008E_CAMERA_H
#include "ray.h"

Vector4D randomInUnitDisk()
{
    Vector4D p;
    do{
        p = Vector4D(xorShift(), xorShift(), 0, 1)*2.0 - Vector4D(1, 1, 0, 1);
    }while(p.dotProduct(p) >= 1.0);

    return p;
}

class camera {
public:
    camera(Vector4D lookfrom, Vector4D lookat, Vector4D vup, float vfov, float aspect, float aperature, float focusDist, float t0, float t1)
    {
        time0 = t0;
        time1 = t1;
        lensRadius = aperature/2;
        float theta = vfov*M_PI/180;
        float halfHeight = tan(theta/2);
        float halfWidth = aspect * halfHeight;
        origin = lookfrom;
        w = (lookfrom-lookat).normalize();
        u = (vup.crossProduct(w)).normalize();
        v = w.crossProduct(u);
        lowerLeftCorner = origin - u*halfWidth*focusDist - v*halfHeight*focusDist - w*focusDist;
        horizontal = u*halfWidth*focusDist*2;
        vertical = v*halfHeight*focusDist*2;

    }
    ray getRay(float s, float t)
    {
        Vector4D rd = randomInUnitDisk()*lensRadius;
        Vector4D offset = u*rd[0] + v*rd[1];
        float time = time0 + drand48()*(time1-time0);
        return ray(origin + offset, lowerLeftCorner + horizontal*s + vertical*t - origin - offset, time);
    }
    Vector4D origin;
    Vector4D lowerLeftCorner;
    Vector4D horizontal;
    Vector4D vertical;
    Vector4D u, v, w;
    float lensRadius;
    float time0, time1;
};

#endif //S0008E_CAMERA_H
