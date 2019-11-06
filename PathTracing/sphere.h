#ifndef S0008E_SPHERE_H
#define S0008E_SPHERE_H
#include "hitable.h"
#include "material.h"

class sphere : public hitable
{
public:
    sphere(){};
    sphere(Vector4D cen, float r, material* mat) : center(cen), radius(r), matPtr(mat){}
    ~sphere(){delete matPtr;}
    virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const;
    Vector4D center;
    float radius;
    material* matPtr;
};
bool sphere::hit(const ray &r, float tmin, float tmax, hitRecord &rec) const
{
    Vector4D oc = r.origin() -  center;
    float a = r.direction().dotProduct(r.direction());
    float b = (oc.dotProduct(r.direction()));
    float c = oc.dotProduct(oc) - radius*radius;
    float d = b*b - a*c;
    if(d > 0)
    {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p-center)/radius;
            rec.matPtr = this->matPtr;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p-center)/radius;
            rec.matPtr = this->matPtr;
            return true;
        }
    }
    return false;
}


#endif //S0008E_SPHERE_H
