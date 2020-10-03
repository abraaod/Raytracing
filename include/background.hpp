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
    Background(std::string type, Vec color);
    Background(std::string type, Vec bl, Vec br, Vec tl, Vec tr, std::string mapping);
    ~Background();
    std::string getType();
    std::string getMapping();
    Vec sample(float v1, float v2);
    Vec interpolate(float x, float y);
};

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


std::string Background::getMapping(){
    return mapping;
}

std::string Background::getType(){
    return type;
}

Vec Background::sample(float v1, float v2){
    if(!color.isBlack()){
        return color;
    } else{
        Vec interpolated = interpolate(v1, v2);
        return interpolated;
    }
}

Vec Background::interpolate(float x, float y){
    float v1_ = bl.v1*(1-x)*(1-y) + br.v1*x*(1-y) + tl.v1*(1-x)*y + tr.v1*x*y;
    float v2_ = bl.v2*(1-x)*(1-y) + br.v2*x*(1-y) + tl.v2*(1-x)*y + tr.v2*x*y;
    float v3_ = bl.v3*(1-x)*(1-y) + br.v3*x*(1-y) + tl.v3*(1-x)*y + tr.v3*x*y;

    return Vec(v1_/255, v2_/255, v3_/255);
}



#endif
