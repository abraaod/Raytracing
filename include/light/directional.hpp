#ifndef _DIRECTIONAL_
#define _DIRECTIONAL_

#include <string>
#include "../vec.hpp"
#include "light.hpp"
#include "../blinnmaterial.hpp"
#include "math.h"

class DirectionalLight : public Light{
    public:

    Vec l;
    Vec scale;
    Vec from;
    Vec to;

    DirectionalLight(std::string type, Vec l, Vec scale, Vec from, Vec to) : Light(type){
        this->type = type;
        this->l = l;
        this->scale = scale;
        this->from = from;
        this->to = to;
    }

    Vec sample_Li(const Surfel& hit /*in*/, Vec v, Vec *wi);

};

Vec DirectionalLight::sample_Li(const Surfel& hit, Vec v, Vec *wi){
    Vec l_ = from - to;
    l_ = normalize(l_);
    *wi = l;
    return l_;
    //return l;

}

#endif