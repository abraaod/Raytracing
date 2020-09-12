#ifndef _FILM_
#define _FILM_

#include <vector>
#include "./vec.hpp"

class Film
{
private:
    /* data */
    std::string filename;
    std::string img_type;
    std::string type;
    int x_res;
    int y_res;
    std::vector<std::vector<Vec>> output;

public:
    Film();
    Film(std::string filename, std::string img_type, std::string type, int x_res, int y_res);
    ~Film();
    int width();
    int height();
    void fillEmpty();
    void add(int x, int y, Vec color);
};

Film::Film(){

}

void Film::fillEmpty(){
    for(int i =0; i < x_res; i++){
        std::vector<Vec> vector;
        for(int j=0; j < y_res; j++){
            vector.push_back(Vec("0 0 0"));
        }
        output.push_back(vector);
    }
}

Film::Film(std::string filename, std::string img_type, std::string type, int x_res, int y_res)
{
    this->filename = filename;
    this->img_type = img_type;
    this->type = type;
    this->x_res = x_res;
    this->y_res = y_res;
    fillEmpty();
}

Film::~Film() = default;

int Film::width(){
    return x_res;
}

int Film::height(){
    return y_res;
}

void Film::add(int x, int y, Vec color){
    output[x][y] = color;
}


#endif
