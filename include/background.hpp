#ifndef _BACKGROUND_
#define _BACKGROUND_

#include <string>
#include "vec.hpp"

class Background
{
private:
    /* data */

    std::string type;
    Vec bl;
    Vec br;
    Vec tl;
    Vec tr;
    Vec color;
    std::string mapping;

public:
    Background();
    Background(std::string type, Vec color);
    Background(std::string type, Vec bl, Vec br, Vec tl, Vec tr, std::string mapping);
    ~Background();
};

Background::Background(){

}

Background::Background(std::string type, Vec color)
{
    this->type = type;
    this->color = color;
}

Background::Background(std::string type, Vec bl, Vec br, Vec tl, Vec tr, std::string mapping){
    this->type = type;
    this->bl = bl;
    this->br = br;
    this->tl = tl;
    this->tr = tr;
    this->mapping = mapping;
}

Background::~Background() = default;

#endif
