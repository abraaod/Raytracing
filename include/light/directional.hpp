#ifndef _AMBIENT_
#define _AMBIENT_

#include <string>
#include "../vec.hpp"
#include "light.hpp"

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
};

#endif