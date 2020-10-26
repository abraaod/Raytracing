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

    Vec sample_Li(const Surfel& hit /*in*/, Vec v);

};

Vec DirectionalLight::sample_Li(const Surfel& hit, Vec v){
    Vec dir = from - to;
    dir = normalize(dir);

    Vec dir_ = v - normalize(hit.p);
    dir_ = normalize(dir_);
    //v = normalize(v);
    Vec n =  normalize(hit.n);
    Vec h =  (dir_ + dir)/(magnitude(dir_+dir));// * magnitude(dir_))); 

    BlinnMaterial *bm = dynamic_cast< BlinnMaterial *>( hit.primitive->get_material());
    Vec c = (bm->kd() * l * std::max(0.f, dot(n, dir))) +  (bm->ks() * l *  std::pow(std::max(0.f, dot(n, h)), bm->glossiness));
    
   
    
    return c;
    //return l;

}

#endif