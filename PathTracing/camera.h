#ifndef S0008E_CAMERA_H
#define S0008E_CAMERA_H
#include "ray.h"
class camera {
public:
    camera(float vfov, float aspect)
    {
        float theta = vfov*M_PI/180;
        float halfHeight = tan(theta/2);
        float halfWidth = aspect * halfHeight;
        lowerLeftCorner = Vector4D(-halfWidth, -halfHeight, -1.0, 1);
        horizontal = Vector4D(2*halfWidth, 0.0, 0.0, 1);
        vertical = Vector4D(0.0, 2*halfHeight, 0.0, 1);
        origin = Vector4D(0.0, 0.0, 0.0, 1);
    }
    ray getRay(float u, float v) { return ray(origin, lowerLeftCorner + horizontal*u + vertical*v - origin);}
    Vector4D origin;
    Vector4D lowerLeftCorner;
    Vector4D horizontal;
    Vector4D vertical;
};

#endif //S0008E_CAMERA_H
