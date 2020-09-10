#ifndef _API_
#define _API_


#include <string>
#include "camera.hpp"
#include "film.hpp"
#include "background.hpp"
#include "paramset.hpp"

class Api
{
private:
    /* data */
    Camera * camera;

    Film * film;

    Background * background;

public:
    Api(/* args */);
    ~Api();

    Camera CAMERA (const Paramset<std::string, std::string> &ps);

    Film FILM(const Paramset<std::string, std::string> &ps);

    Background BACKGROUND(const Paramset<std::string, std::string> &ps);

};

Api::Api(/* args */)
{
}

Api::~Api() = default;

Camera Api::CAMERA(const Paramset<std::string, std::string> &ps){
    std::string type = ps.find("type");
    camera = new Camera(type);
}

Film Api::FILM(const Paramset<std::string, std::string> &ps){
        std::string filename = ps.find("filename");
        std::string img_type = ps.find("img_type");
        std::string type = ps.find("type");
        int x_res = std::stoi(ps.find("x_res"));
        int y_res = std::stoi(ps.find("y_res"));
        film = new Film(filename, img_type, type, x_res, y_res);
}

Background Api::BACKGROUND(const Paramset<std::string, std::string> &ps){

}

#endif