#include <iostream>
#include "Vector4D.h"
#include <fstream>
#include "ray.h"
#include <math.h>



Vector4D color(const ray& r)
{
    float t = hitSphere(Vector4D(0,0,-1,1), 0.5, r);
    if(t > 0.0)
    {
        Vector4D N = (r.pointAtParameter(t) - Vector4D(0,0,-1,1)).normalize();
        return (Vector4D(N[0]+1, N[1]+1, N[2]+1, 1)*0.5);
    }

    Vector4D unitDirection = r.direction().normalize();
    t = 0.5*(unitDirection[1] + 1.0);
    return Vector4D(1.0,1.0,1.0,1)*(1.0-t) + Vector4D(0.5,0.7,1.0,1)*t;

}

int main() {
    std::ofstream file;
    file.open("image.ppm");
    int nx = 200;
    int ny = 100;
    file << "P3\n" << nx << " " << ny << "\n255\n";
    Vector4D lowerLeftCorner(-2.0, -1.0, -1.0, 1);
    Vector4D horizontal(4.0, 0,0,1);
    Vector4D vertical(0.0,2.0,0.0,1);
    Vector4D origin(0.0,0.0,0.0,1);
    for (int j = ny -1; j >= 0 ; j--)
    {
        for (int i = 0; i < nx; ++i)
        {
            float u = float(i)/float(nx);
            float v = float(j)/float(ny);
            ray r(origin, lowerLeftCorner + horizontal*u + vertical*v );
            Vector4D vec = color(r);
            int ir = int(255.99*vec[0]);
            int ig = int(255.99*vec[1]);
            int ib = int(255.99*vec[2]);
            file << ir << " " << ig << " " << ib << "\n";
        }
    }
    file.close();
}