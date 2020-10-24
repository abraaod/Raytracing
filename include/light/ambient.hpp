#ifndef _AMBIENT_
#define _AMBIENT_

#include <string>
#include "../vec.hpp"
#include "light.hpp"

class AmbientLight : public Light{
    public:

    Vec l;

    AmbientLight(std::string type, Vec l) : Light(type){
        this->type = type;
        this->l = l;
    }
};

#endif