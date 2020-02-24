#ifndef S0008E_ONB_H
#define S0008E_ONB_H

#include "Vector4D.h"

class onb
{
public:
    onb() {}
    inline Vector4D operator[](int i) const {return axis[i];}
    Vector4D u() const {return axis[0];}
    Vector4D v() const {return axis[1];}
    Vector4D w() const {return axis[2];}
    Vector4D local(float a, float b, float c) const { return u()*a + v()*b + w()*c;}
    Vector4D local(const Vector4D& a) const {return u()*a[0] + v()*a[1] + w()*a[2];}
    void buildFromW(const Vector4D&);
    Vector4D axis[3];
};

void onb::buildFromW(const Vector4D& n)
{
    axis[2] = n.normalize();
    Vector4D a;
    if(fabs(w()[0]) > 0.9)
        a = Vector4D(0, 1, 0, 1);
    else
        a = Vector4D(1, 0, 0, 1);
    axis[1] = (w().crossProduct(a)).normalize();
    axis[0] = w().crossProduct(v());
}


#endif //S0008E_ONB_H
