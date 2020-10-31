#ifndef _LIGHT_
#define _LIGHT_

#include <string>
#include "../surfel.hpp"
#include "../vec.hpp"
#include "../scene.hpp"

class Scene;

class Light{
    public:
    std::string type;
    Vec from;

    Light(std::string type){this->type = type;}
    Light(std::string type, Vec from){this->from = from; this->type = type;}
    virtual Vec sample_Li(const Surfel& hit /*in*/, Vec v, Vec *wi) = 0;
    void preprocessLight( Scene & scene) {};

};

#endif