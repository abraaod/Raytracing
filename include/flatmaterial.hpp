#ifndef _FLATMATERIAL_
#define _FLATMATERIAL_

#include <string>
#include "material.hpp"
#include "vec.hpp"

using Color24 = Vec;

class FlatMaterial : public Material{
    public:
    Color24 color;

    ~FlatMaterial(){};

    FlatMaterial(Vec color);
    void scatter( Surfel *isect, Ray *r);
    Color24 kd();
};

FlatMaterial::FlatMaterial(Vec color){
    this->color = color;
}

void FlatMaterial::scatter( Surfel *isect, Ray *r){
    
}

Color24 FlatMaterial::kd(){
    return color;
}

#endif