#ifndef S0008E_RAY_H
#define S0008E_RAY_H
#include "Vector4D.h"
class ray
{
public:
    ray(){}
    ray(const Vector4D& a, const Vector4D& b) {A = a; B = b;}
    Vector4D origin() const {return A;}
    Vector4D direction() const {return B;}
    Vector4D pointAtParameter(float t) const {return A + (B * t);}
    Vector4D A;
    Vector4D B;

};
#endif //S0008E_RAY_H
