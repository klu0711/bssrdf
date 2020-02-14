#ifndef S0008E_TEXTURE_H
#define S0008E_TEXTURE_H
#include "Vector4D.h"

class texture
{
public:
    virtual Vector4D value(float u, float v, const Vector4D& p) const = 0;
};

class constantTexture : public texture
{
public:
    constantTexture(){}
    constantTexture(Vector4D c) : color(c){}
    virtual Vector4D value(float u, float v, const Vector4D& p) const
    {
        return color;
    }
    Vector4D color;
};

class checker_texture : public texture
{
public:
    checker_texture(){}
    checker_texture(texture* t0, texture* t1): even(t0), odd(t1){}
    virtual Vector4D value(float u, float v, const Vector4D& p) const
    {
        float sines = sin(10 * p[0]) * sin(10*p[1]) * sin(10*p[2]);
        if(sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
    texture* even;
    texture* odd;
};

#endif //S0008E_TEXTURE_H