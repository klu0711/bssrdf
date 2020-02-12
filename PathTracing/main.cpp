#include <iostream>
#include "Vector4D.h"
#include <fstream>
#include <chrono>
#include <assert.h>
#include <algorithm>
#include "ray.h"
#include "hitableList.h"
#include "sphere.h"
#include "camera.h"
#include <thread>
#include <vector>
#include "node.h"



hitable* random_scene(int spheres)
{
    hitable **list = new hitable*[spheres+4];
    list[0] = new sphere(Vector4D(0, -1000, 0, 1), 1000, new lambertian(Vector4D(0.5, 0.5, 0.5, 1)));
    int i = 1;

    for (int b = 0; b < spheres; ++b)
    {
        float chooseMat = xorShift();
        Vector4D center(20*(xorShift() - 0.5),0.2, 20*(xorShift() - 0.5), 1 );
        if((center-Vector4D(4, 0.2, 0, 1)).length() > 0.9)
        {
            if(chooseMat < 0.8) //Diffuse
            {
                //list[i++] = new sphere(center, 0.2, new lambertian(Vector4D(xorShift()*xorShift(), xorShift()*xorShift(), xorShift()*xorShift(), 1)));
                list[i++] = new movingSphere(center, center + Vector4D(0, 0.5*xorShift(), 0, 1), 0.0, 1.0, 0.2, new lambertian(Vector4D(xorShift()*xorShift(), xorShift()*xorShift(), xorShift()*xorShift(), 1)));

            }else if(chooseMat < 0.95) // metal
            {
                list[i++] = new sphere(center, 0.2,
                        new metal(Vector4D(0.5*(1+xorShift()), 0.5*(1+xorShift()), 0.5*(1 + xorShift()), 1), 0.5*xorShift()));
            }else
            {
                list[i++] = new sphere(center, 0.2, new dielectric(1.5));
            }
        }

    }
    list[i++] = new sphere(Vector4D(0, 1, 0, 1), 1.0, new dielectric(1.5));
    list[i++] = new sphere(Vector4D(-4, 1, 0, 1), 1.0, new lambertian(Vector4D(0.4, 0.2, 0.1, 1)));
    list[i++] = new sphere(Vector4D(4, 1, 0, 1), 1.0, new metal(Vector4D(0.7, 0.6, 0.5, 1), 0.0));
    return new hitableList(list, i);
}

hitable* generateScene()
{
    int spheres = 100;
    hitable **list = new hitable*[101];
    hitable ** sphereList = new hitable*[spheres];
    hitable ** sphereList2 = new hitable*[spheres];
    int b = 0;
    for (int i = 0; i < spheres; ++i)
    {
        Vector4D center(20*(xorShift() - 0.5),0.2, 20*(xorShift() - 0.5), 1 );
        if((center-Vector4D(4, 0.2, 0, 1)).length() > 0.9)
        {
            sphereList[b++] = new sphere(center, 0.2, new lambertian(Vector4D(xorShift()*xorShift(), xorShift()*xorShift(), xorShift()*xorShift(), 1)));
        }
    }

    //sphereList[b++] = new sphere(Vector4D(0, -1000, 0, 1), 1000, new lambertian(Vector4D(0.5, 0.5, 0.5, 1)));
    list[0] = new bvhNode(sphereList, b, 0, 1);
    int a = 1;
    for (int i = 0; i < 10; ++i)
    {
        Vector4D center(20*(xorShift() - 0.5),0.2, 20*(xorShift() - 0.5), 1 );

       // list[a++] = new sphere(center, 0.2, new lambertian(Vector4D(xorShift()*xorShift(), xorShift()*xorShift(), xorShift()*xorShift(), 1)));

    }

    return new hitableList(list, 1);
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
struct pixel
{
    int x, y;
    int r;
    int g;
    int b;
};

struct pixelContainer
{
    int currentAvailable = 0;
    std::vector<pixel> pixels;
    int numPixels;
    pixel* getPixel()
    {
        pixel* returnValue;
        if(currentAvailable >= (numPixels - 1))
        {
            returnValue = nullptr;
            currentAvailable++;
        }else
        {
            returnValue = &pixels[currentAvailable++];
        }
        return returnValue;
    }
};
pixelContainer cont;
unsigned int numRays;
void calcPixel(int ns, hitable * world, int nx, int ny, camera cam)
{
    while(1)
    {
        pixel* currentPixel = cont.getPixel();
        if(currentPixel == nullptr)
            break;
        Vector4D col(0,0,0,1);
        for (int i = 0; i < ns; ++i)
        {
            float u = float(currentPixel->x + xorShift())/float(nx);
            float v = float(currentPixel->y + xorShift())/float(ny);
            ray r = cam.getRay(u, v);
            Vector4D p = r.pointAtParameter(2.0);
            col = col + color(r, world, 0);
            numRays++;

        }
        col = col / float(ns);
        col = Vector4D(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]), 1);
        currentPixel->r = int(255.99*col[0]);
        currentPixel->g = int(255.99*col[1]);
        currentPixel->b = int(255.99*col[2]);
    }
}


int main(int argCount, char* argVector[]) {

    auto start = std::chrono::system_clock::now();
    int nx, ny, ns, sp;
    nx = 200;
    ny = 100;
    ns = 10;
    sp = 10;
    std::ofstream file;
    file.open("image.ppm");
    file << "P3\n" << nx << " " << ny << "\n255\n";
    Vector4D lowerLeftCorner(-2.0, -1.0, -1.0, 1);
    Vector4D horizontal(4.0, 0,0,1);
    Vector4D vertical(0.0,2.0,0.0,1);
    Vector4D origin(0.0,0.0,0.0,1);

    hitable *world1 = random_scene(sp);
    hitable *world = generateScene();
    float R = cos(M_PI/4);
    Vector4D lookfrom(13, 2, 3, 1);
    Vector4D lookat(0, 0, 0, 1);
    float distToFocus = 10;
    float aperature = 0.1;
    camera cam(lookfrom, lookat, Vector4D(0, 1, 0, 1), 20, float(nx)/ny, aperature, distToFocus, 0.0f, 1.0f);

    cont.pixels.resize(ny*nx);
    cont.numPixels = nx*ny;

    for (int l = ny - 1; l >= 0 ;l--)
    {
        for (int i = 0; i < nx; ++i)
        {
            int nYnX = l*nx + i;
            cont.pixels[nYnX].x = i;
            cont.pixels[nYnX].y = l;

        }
    }
    int numThreads = 12;
    std::thread threads[numThreads];
    for (int m = 0; m < numThreads; ++m)
    {
        threads[m] = std::thread(calcPixel, ns, world, nx, ny, cam);
    }
    for (int j = 0; j < numThreads; ++j)
    {
        threads[j].join();
    }


   for (int j =  ny - 1; j >= 0 ; j--)
   {
        for (int i = 0; i < nx; ++i)
        {
            file << cont.pixels[j*nx + i].r << " " << cont.pixels[j*nx + i].g << " " << cont.pixels[j*nx + i].b << "\n";
        }
    }


   /* for (int j = ny -1; j >= 0 ; j--)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vector4D col(0,0,0,1);
            for (int k = 0; k < ns; ++k)
            {
                float u = float(i + xorShift())/float(nx);
                float v = float(j + xorShift())/float(ny);
                ray r = cam.getRay(u, v);
                numRays++;
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
    }*/
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Rays: " << (float)numRays/(float)1000000 << " M" << std::endl << "Elapsed time: " << elapsed.count() << " Seconds" << std::endl;

    delete world;
    file.close();
}


