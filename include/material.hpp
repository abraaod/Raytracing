#ifndef _MATERIAL_
#define _MATERIAL_

#include "./surfel.hpp"
#include "./ray.hpp"
#include "vec.hpp"

using Color24 = Vec;
class Material{
    public:

    virtual void scatter(Surfel * isect, Ray *r) = 0;
    virtual Vec kd() = 0;
};

#endif