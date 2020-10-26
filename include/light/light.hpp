#ifndef _LIGHT_
#define _LIGHT_

#include <string>
#include "../surfel.hpp"
#include "../vec.hpp"

class Light{
    public:
    std::string type;

    Light(std::string type){this->type = type;}

    virtual Vec sample_Li(const Surfel& hit /*in*/, Vec v) = 0;

};

#endif