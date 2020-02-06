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
    virtual bool boundingBox(float t0, float t1, aabb& box) const;
    Vector4D center;
    float radius;
    material* matPtr;
};

bool sphere::boundingBox(float t0, float t1, aabb &box) const
{
    box = aabb(center - Vector4D(radius, radius, radius, 1), center + Vector4D(radius, radius, radius, 1));
    return true;
}

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

class movingSphere : public hitable
{
public:
    movingSphere(){}
    movingSphere(Vector4D cen0, Vector4D cen1, float t0, float t1, float r, material *m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), matPtr(m) {};
    virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const;
    Vector4D center(float time) const;

    aabb surroundingBox(aabb box0, aabb box1)
    {
        Vector4D small(fmin(box0.min()[0], box1.min()[0]),
                       fmin(box0.min()[1], box1.min()[1]),
                       fmin(box0.min()[2], box1.min()[2]),1);

        Vector4D big(fmin(box0.max()[0], box1.max()[0]),
                     fmin(box0.max()[1], box1.max()[1]),
                     fmin(box0.max()[2], box1.max()[2]),1);
        return aabb(small, big);
    }

    Vector4D center0, center1;
    float time0, time1;
    float radius;
    material *matPtr;
};

Vector4D movingSphere::center(float time) const
{
    return center0 +  (center1 - center0) * ((time - time0) / (time1 + time0));
}

bool movingSphere::hit(const ray &r, float tmin, float tmax, hitRecord &rec) const
{
    Vector4D oc = r.origin() -  center(r.time());
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
            rec.normal = (rec.p-center(r.time()))/radius;
            rec.matPtr = this->matPtr;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if(temp < tmax && temp > tmin)
        {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p-center(r.time()))/radius;
            rec.matPtr = this->matPtr;
            return true;
        }
    }
    return false;
}


#endif //S0008E_SPHERE_H
