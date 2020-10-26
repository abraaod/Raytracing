#ifndef _BLINNMATERIAL_
#define _BLINNMATERIAL_

#include "material.hpp"

using Color24 = Vec;

class BlinnMaterial : public Material{

    public:
    Color24 color_kd;
    Color24 color_ks;
    Color24 color_ka;
    std::string name;
    Vec mirror;
    float glossiness;

    ~BlinnMaterial(){};

    BlinnMaterial(Vec kd, Vec ks, Vec ka, std::string name, Vec mirror, float glossiness);
    void scatter( Surfel *isect, Ray *r);
    Color24 kd();
    Color24 ks();
    Color24 ka();
};

BlinnMaterial::BlinnMaterial(Vec kd, Vec ks, Vec ka, std::string name, Vec mirror, float glossiness){
    this->color_kd = kd;
    this->color_ks = ks;
    this->color_ka = ka;
    this->name = name;
    this->mirror = mirror;
    this->glossiness = glossiness;
}

void BlinnMaterial::scatter( Surfel *isect, Ray *r){
    
}

Color24 BlinnMaterial::kd(){
    return color_kd;
}

Color24 BlinnMaterial::ks(){
    return color_ks;
}

Color24 BlinnMaterial::ka(){
    return color_ka;
}

#endif