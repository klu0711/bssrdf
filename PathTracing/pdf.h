#ifndef S0008E_PDF_H
#define S0008E_PDF_H

#include "Vector4D.h"
#include "onb.h"
#include "material.h"
class pdf
{
public:
    virtual float value(const Vector4D& dir) const = 0;
    virtual Vector4D generate() const = 0;
};


class cosinePdf : public pdf
{
public:
    cosinePdf(const Vector4D& w) { uvw.buildFromW(w);}
    virtual float value(const Vector4D& dir) const
    {
        float cosine = dir.normalize().dotProduct(uvw.w());
        if(cosine > 0)
        {
            return cosine / RPI;
        } else
        {
            return 0;
        }
    }
    virtual Vector4D generate() const
    {
        return  uvw.local(randomCosineDir());
    }

    onb uvw;
};

class hitablePdf : public pdf
{
public:
    hitablePdf(hitable *p, const Vector4D& origin) : ptr(p), o(origin) {}
    virtual float value(const Vector4D& dir) const
    {
        return ptr->pdfValue(o, dir);
    }
    virtual Vector4D generate() const
    {
        return ptr->random(o);
    }
    Vector4D o;
    hitable* ptr;
};

class mixturePdf : public pdf
{
public:
    mixturePdf(pdf* p0, pdf* p1) { p[0] = p0; p[1] = p1; }
    virtual float value(const Vector4D& dir) const
    {
        return 0.5f * p[0]->value(dir) + 0.5f * p[1]->value(dir);
    }

    virtual Vector4D generate() const
    {
        if(drand48() < 0.5f)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

    pdf* p[2];
};

#endif //S0008E_PDF_H
