#ifndef _RAY_
#define _RAY_

#include "./vec.hpp"

using Point3 = Vec;

class Ray{
    private:
        Point3 o;
        Vec d;

    public:
        Point3 operator()(float t){return o + d * t;}
        Ray(const Point3& o, const Vec& d) : o{o}, d{d} {/*empty*/}
};

#endif