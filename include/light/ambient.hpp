#ifndef _AMBIENT_
#define _AMBIENT_

#include <string>
#include "../vec.hpp"
#include "../scene.hpp"
#include "light.hpp"

class Scene;

class AmbientLight : public Light{
    public:

    Vec l;

    AmbientLight(std::string type, Vec l) : Light(type){
        this->type = type;
        this->l = l;
    }

    Vec sample_Li(const Surfel& hit /*in*/, Vec v, Vec *wi);
    void preprocessLight( Scene & scene){

    }
};

Vec AmbientLight::sample_Li(const Surfel& hit, Vec v, Vec *wi){
    return l;
}

#endif