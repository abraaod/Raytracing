#ifndef _FLATMATERIAL_
#define _FLATMATERIAL_

#include <string>
#include "material.hpp"
#include "vec.hpp"

class FlatMaterial : public Material{
    public:
    std::string type;
    Vec color;

    ~FlatMaterial(){};

    FlatMaterial(Vec color);

};

FlatMaterial::FlatMaterial(Vec color){
    this->type = "flat";
    this->color = color;
}



#endif