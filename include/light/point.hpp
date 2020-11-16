#ifndef _POINT_
#define _POINT_

#include <string>
#include "../vec.hpp"
#include "./light.hpp"
#include "../scene.hpp"


class PointLight : public Light{
    public:

    Vec i;
    Vec scale;
    // Vec from;

    PointLight(std::string type, Vec i, Vec scale, Vec from) : Light(type, from){
        this->type = type;
        this->i = i;
        this->scale = scale;
        this->from = from;
    }

    Vec sample_Li(const Surfel& hit /*in*/, Vec v, Vec *wi) {

        Vec l = from - hit.p;
        l = normalize(l);
        *wi = i;
        return l;

    }

    void preprocessLight(Scene & scene){

    }
    
};

#endif