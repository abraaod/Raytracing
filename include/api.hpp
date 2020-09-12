#ifndef _API_
#define _API_


#include <string>
#include "camera.hpp"
#include "film.hpp"
#include "background.hpp"
#include "paramset.hpp"
#include "vec.hpp"

class Api
{
private:
    /* data */
    Camera camera;

    Film film;

    Background background;

public:
    Api(/* args */);
    ~Api();

    void CAMERA (Paramset<std::string, std::string> ps);

    void FILM(Paramset<std::string, std::string> ps);

    void BACKGROUND(Paramset<std::string, std::string> ps);

    Camera getCamera();

    Film getFilm();

    Background getBackground();

};

Api::Api(/* args */)
{
}

Api::~Api() = default;

void Api::CAMERA(Paramset<std::string, std::string> ps){
    std::string type = ps.find("type");
    Camera c(type);
    camera = c;
}

void Api::FILM(Paramset<std::string, std::string> ps){
        std::string filename = ps.find("filename");
        std::string img_type = ps.find("img_type");
        std::string type = ps.find("type");
        int x_res = std::stoi(ps.find("x_res"));
        int y_res = std::stoi(ps.find("y_res"));
        Film f(filename, img_type, type, x_res, y_res);
        film = f;
}

void Api::BACKGROUND(Paramset<std::string, std::string> ps){
    std::string type = ps.find("type");
    std::string color(ps.find("color"));
    if(color != ""){
        Vec c(color);
        Background b(type, c);
        background = b;
    } else {
        Vec bl(ps.find("bl"));
        Vec br(ps.find("br"));
        Vec tl(ps.find("tl"));
        Vec tr(ps.find("tr"));
        std::string mapping = ps.find("mapping");
        Background b(type, bl, br, tl, tr, mapping);
        background = b;
    }
}

Camera Api::getCamera(){
    return this->camera;
}

Film Api::getFilm(){
    return this->film;
}

Background Api::getBackground(){
    return this->background;
}

#endif