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
#include "texture.h"
#include "rect.h"
#include "diffuseLight.h"
#include "box.h"
#include "translate.h"
#include <iostream>
#include "pdf.h"


inline Vector4D deNan(const Vector4D& c)
{
    Vector4D temp = c;
    if(!(temp[0] == temp[0])) temp[0] = 0;
    if(!(temp[1] == temp[1])) temp[1] = 0;
    if(!(temp[2] == temp[2])) temp[2] = 0;
    return temp;
}


hitable* generateScene()
{
    int spheres = 100;
    hitable **list = new hitable*[103];
    hitable ** sphereList = new hitable*[spheres];
    hitable ** sphereList2 = new hitable*[spheres];
    int b = 0;

    texture* checker = new checker_texture(new constantTexture(Vector4D(0.2, 0.3, 0.1, 1)), new constantTexture(Vector4D(0.9, 0.9, 0.9, 1)));

    for (int i = 0; i < spheres; ++i)
    {
        Vector4D center(20*(xorShift() - 0.5),0.2, 20*(xorShift() - 0.5), 1 );
        if((center-Vector4D(4, 0.2, 0, 1)).length() > 0.9)
        {
            sphereList[b++] = new sphere(center, 0.2, new lambertian(new constantTexture(Vector4D(xorShift()*xorShift(), xorShift()*xorShift(), xorShift()*xorShift(), 1))));
        }
    }

    list[0] = new bvhNode(sphereList, b, 0, 1);
    list[1] = new sphere(Vector4D(0, -1000, 0, 1), 1000, new lambertian(checker));
    list[2] = new xyRect(3, 5, 1, 3, -2, new diffuseLight(new constantTexture(Vector4D(4, 4, 4, 1))));


    return new hitableList(list, 3);
}

hitable* cornellBox()
{
    hitable** list = new hitable*[6];
    int i = 0;
    material* red = new lambertian(new constantTexture(Vector4D(0.65, 0.05, 0.05, 1)));
    material* white = new lambertian(new constantTexture(Vector4D(0.73, 0.73, 0.73, 1)));
    material* green = new lambertian(new constantTexture(Vector4D(0.12, 0.45, 0.15, 1)));
    material* light = new diffuseLight(new constantTexture(Vector4D(15, 15, 15, 1)));
    material* metalMat = new metal(Vector4D(0.8, 0.85, 0.88, 0), 0.0);
    list[i++] = new flipNormal(new yzRect(0, 555, 0, 555, 555, green));
    list[i++] = new yzRect(0, 555, 0, 555, 0, red);
    list[i++] = new flipNormal(new xzRect(213, 343, 227, 332, 554, light));
    list[i++] = new flipNormal(new xzRect(0, 555, 0, 555, 555, white));
    list[i++] = new xzRect(0, 555, 0, 555, 0, white);
    list[i++] = new flipNormal(new xyRect(0, 555, 0, 555, 555, white));
    list[i++] = new translate(new rotateY(new box(Vector4D(0, 0, 0, 1), Vector4D(165, 165, 165, 1), white), -18), Vector4D(130, 0, 65, 1));
    list[i++] = new translate(new rotateY(new box(Vector4D(0, 0, 0, 1), Vector4D(165, 330, 165, 1), white), 15), Vector4D(265, 0, 295, 1));

    return new hitableList(list, i);
}

Vector4D color(const ray& r, hitable *world, hitable* lightShape,  int depth)
{
    hitRecord hRec;
    if(world->hit(r, 0.001, MAXFLOAT, hRec))
    {
        scatterRecord sRec;
        //Only lights emit light
        Vector4D emitted = hRec.matPtr->emitted(r, hRec, hRec.u, hRec.v, hRec.p);
        //Scatter returns false when it hits a light
        if(depth < 50 && hRec.matPtr->scatter(r, hRec, sRec))
        {
            if(sRec.isSpecular)
            {
                return sRec.attenuation * color(sRec.specularRay, world, lightShape, depth+1);
            } else
            {
                hitablePdf pLight(lightShape, hRec.p);
                mixturePdf p(&pLight, sRec.pdfPtr);
                ray scattered = ray(hRec.p, p.generate(), r.time());
                float pdfVal = p.value(scattered.direction());
                return emitted + sRec.attenuation*hRec.matPtr->scatterPdf(r, hRec, scattered) * color(scattered, world, lightShape, depth + 1) / pdfVal;
            }
        }else
        {
            return emitted;
        }
    }else
    {
        return Vector4D(0, 0, 0, 1);

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
    hitable* lightShape = new xzRect(213, 343, 227, 332, 554, 0);

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
            col = col + deNan(color(r, world, lightShape,0));
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
    nx = 400;
    ny = 400;
    ns = 500;
    sp = 10;

    Vector4D lowerLeftCorner(-2.0, -1.0, -1.0, 1);
    Vector4D horizontal(4.0, 0,0,1);
    Vector4D vertical(0.0,2.0,0.0,1);
    Vector4D origin(0.0,0.0,0.0,1);

    //hitable *world1 = random_scene(sp);
    hitable *world = cornellBox();
    float R = cos(M_PI/4);
    //Vector4D lookfrom(13, 2, 3, 1);
    //Vector4D lookat(0, 0, 0, 1);
    //float distToFocus = 10;
    //float aperature = 0.1;
    Vector4D lookfrom(278, 278, -750, 1);
    Vector4D lookat(278, 278, 0, 1);
    float distToFocus = 10;
    float aperature = 0;

    camera cam(lookfrom, lookat, Vector4D(0, 1, 0, 1), 40, float(nx)/ny, aperature, distToFocus, 0.0f, 1.0f);

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
    int numThreads = 8;
    std::thread threads[numThreads];
    for (int m = 0; m < numThreads; ++m)
    {
        threads[m] = std::thread(calcPixel, ns, world, nx, ny, cam);
    }
    for (int j = 0; j < numThreads; ++j)
    {
        threads[j].join();
    }

    std::ofstream file;
    file.open("image.ppm");
    file << "P3\n" << nx << " " << ny << "\n255\n";
   for (int j =  ny - 1; j >= 0 ; j--)
   {
        for (int i = 0; i < nx; ++i)
        {
            file << cont.pixels[j*nx + i].r << " " << cont.pixels[j*nx + i].g << " " << cont.pixels[j*nx + i].b << "\n";
        }
    }


    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Rays: " << (float)numRays/(float)1000000 << " M" << std::endl << "Elapsed time: " << elapsed.count() << " Seconds" << std::endl;

    delete world;
    file.close();
}


