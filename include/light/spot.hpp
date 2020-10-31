#ifndef _SPOT_
#define _SPOT_

#include <string>
#include "../vec.hpp"
#include "./light.hpp"
#include "../scene.hpp"


class SpotLight : public Light{
    public:

    Vec i;
    Vec scale;
    Vec from;
    Vec to;
    Vec cutoff;
    Vec falloff;

    SpotLight(std::string type, Vec i, Vec scale, Vec from, Vec to, Vec cutoff, Vec falloff) : Light(type){
        this->i = i;
        this->scale = scale;
        this->from = from;
        this->to = to;
        this->cutoff = cutoff;
        this->falloff = falloff;
    }

    Vec sample_Li(const Surfel& hit /*in*/, Vec v, Vec *wi) {}
    void preprocessLight(Scene & scene){

    }
};

#endif