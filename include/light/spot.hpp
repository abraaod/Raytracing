#ifndef _SPOT_
#define _SPOT_

#include <string>
#include "../vec.hpp"
#include "./light.hpp"

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
};

#endif