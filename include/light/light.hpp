#ifndef _LIGHT_
#define _LIGHT_

#include <string>

class Light{
    public:
    std::string type;

    Light(std::string type){this->type = type;}
};

#endif