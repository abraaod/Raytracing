#ifndef _RAY_
#define _RAY_

#include "./vec.hpp"
#include <iostream>

using Point3 = Vec;

class Ray{
    private:
        Point3 o;
        Vec d;

    public:
        Point3 operator()(float t){return o + d * t;}
        Ray(const Point3& o, const Vec& d) : o{o}, d{d} {/*empty*/}
        ~Ray();
        friend std::ostream& operator<<(std::ostream& os, const Ray& r);

        Vec getOrigin();
        Vec getDirection();
};

std::ostream& operator<<(std::ostream& os, const Ray& r)
{
    os <<  "Origin: " << r.o.v1 << " " << r.o.v2 << " " << r.o.v3 << "\nDirection: " << r.d.v1 << " " << r.d.v2 << " " << r.d.v3 << "\n"; 
    return os;
}

Vec Ray::getOrigin(){
    return o;
}

Vec Ray::getDirection(){
    return d;
}

Ray::~Ray() = default;

#endif