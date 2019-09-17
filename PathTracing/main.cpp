#include <iostream>
#include "Vector4D.h"
#include <fstream>
#include "ray.h"
#include <math.h>
#include "hitableList.h"
#include "sphere.h"
#include "camera.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"

hitable* random_scene()
{
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(Vector4D(0, -1000, 0, 1), 1000, new lambertian(Vector4D(0.5, 0.5, 0.5, 1)));
    int i = 1;
    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            float chooseMat = drand48();
            Vector4D center(a+0.9*drand48(),0.2, b+0.9*drand48(), 1 );
            if((center-Vector4D(4, 0.2, 0, 1)).length() > 0.9)
            {
                if(chooseMat < 0.8) //Diffuse
                {
                    list[i++] = new sphere(center, 0.2, new lambertian(Vector4D(drand48()*drand48(), drand48()*drand48(), drand48()*drand48(), 1)));
                }else if(chooseMat < 0.95) // metal
                {
                    list[i++] = new sphere(center, 0.2,
                            new metal(Vector4D(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1 + drand48()), 1), 0.5*drand48()));
                }else
                {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(Vector4D(0, 1, 0, 1), 1.0, new dielectric(1.5));
    list[i++] = new sphere(Vector4D(-4, 1, 0, 1), 1.0, new lambertian(Vector4D(0.4, 0.2, 0.1, 1)));
    list[i++] = new sphere(Vector4D(4, 1, 0, 1), 1.0, new metal(Vector4D(0.7, 0.6, 0.5, 1), 0.0));
    return new hitableList(list, i);
}

Vector4D color(const ray& r, hitable *world, int depth)
{
    hitRecord rec;
    if(world->hit(r, 0.001, MAXFLOAT, rec))
    {
        ray scattered;
        Vector4D attenuation;
        if(depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered))
        {
            return color(scattered, world, depth + 1)*attenuation;
        }else
        {
            return Vector4D(0,0,0,1);
        }
    }else
    {
        Vector4D unitDirection = r.direction().normalize();
        float t = 0.5*(unitDirection[1] + 1.0);
        return Vector4D(1.0,1.0,1.0,1)*(1.0-t) + Vector4D(0.5,0.7,1.0,1)*t;
    }
}

int main() {
    std::ofstream file;
    file.open("image.ppm");
    int nx = 1000;
    int ny = 500;
    int ns = 500;
    file << "P3\n" << nx << " " << ny << "\n255\n";
    Vector4D lowerLeftCorner(-2.0, -1.0, -1.0, 1);
    Vector4D horizontal(4.0, 0,0,1);
    Vector4D vertical(0.0,2.0,0.0,1);
    Vector4D origin(0.0,0.0,0.0,1);
   // hitable *list[500];
   /* list[0] = new sphere(Vector4D(0,0,-1,1), 0.5, new lambertian(Vector4D(0.8, 0.3, 0.3, 1)));
    list[1] = new sphere(Vector4D(0, -100.5,-1,1), 100, new lambertian(Vector4D(0.8, 0.8, 0.0, 1)));
    list[2] = new sphere(Vector4D(1, 0, -1,1), 0.5, new metal(Vector4D(0.8, 0.6, 0.2, 1), 0.3));
    list[3] = new sphere(Vector4D(-1, 0, -1, 1), 0.5, new dielectric(1.5));
    list[4] = new sphere(Vector4D(-1, 0, -1, 1), -0.45, new dielectric(1.5));*/
    hitable * world = random_scene();
    //hitable *world = new hitableList(list, 5);
    float R = cos(M_PI/4);
    //list[0] = new sphere(Vector4D(-R, 0, -1, 1), R, new lambertian(Vector4D(0,0,1, 1)));
    //list[1] = new sphere(Vector4D(R, 0, -1, 1), R, new lambertian(Vector4D(1,0,0, 1)));
    //hitable *world = new hitableList(list, 2);
    Vector4D lookfrom(13, 2, 3, 1);
    Vector4D lookat(0, 0, 0, 1);
    float distToFocus = 10;
    float aperature = 0.1;
    camera cam(lookfrom, lookat, Vector4D(0, 1, 0, 1), 20, float(nx)/ny, aperature, distToFocus);
    for (int j = ny -1; j >= 0 ; j--)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vector4D col(0,0,0,1);
            for (int k = 0; k < ns; ++k)
            {
                float u = float(i + drand48())/float(nx);
                float v = float(j + drand48())/float(ny);
                ray r = cam.getRay(u, v);
                Vector4D p = r.pointAtParameter(2.0);
                col = col + color(r, world, 0);
            }
            col = col / float(ns);
            col = Vector4D(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]), 1);
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            file << ir << " " << ig << " " << ib << "\n";
        }
    }
    file.close();
}