#ifndef _POINT_
#define _POINT_

#include <string>
#include "../vec.hpp"
#include "./light.hpp"

class PointLight : public Light{
    public:

    Vec i;
    Vec scale;
    Vec from;

    PointLight(std::string type, Vec i, Vec scale, Vec from) : Light(type){
        this->type = type;
        this->i = i;
        this->scale = scale;
        this->from = from;
    }
};

#endif