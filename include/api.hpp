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
    Camera* camera;

    Film* film;

    Background* background;

public:
    Api(/* args */);
    ~Api();

    void CAMERA(Paramset<std::string, std::string> ps);

    void FILM(Paramset<std::string, std::string> ps);

    void BACKGROUND(Paramset<std::string, std::string> ps);

    Camera getCamera();

    Film getFilm();

    Background getBackground();

    void render();

};

Api::Api(/* args */) : camera{nullptr}, film{nullptr}, background{nullptr}
{
}

Api::~Api(){
    if(camera)
        delete camera;
    if(film)
        delete film;
    if(background)
        delete background;
}

void Api::CAMERA(Paramset<std::string, std::string> ps){
    std::string type = ps.find("type");
    Vec lookat(ps.find("look_at"));
    Vec lookfrom(ps.find("look_from"));
    Vec vup(ps.find("up"));
    std::cout << "vup: " << ps.find("up") << std::endl;
    std::string screen = ps.find("screen_window");

    if(type.compare("orthographic") == 0){
        camera =  new OrthograficCamera(type, screen, lookat, lookfrom, vup);
    } else if (type.compare("perspective") == 0){
        float fovy = std::stof(ps.find("fovy"));
        camera = new PerspectiveCamera(type, fovy, lookat, lookfrom, vup);
    }
}

void Api::FILM(Paramset<std::string, std::string> ps){
        std::string filename = ps.find("filename");
        std::string img_type = ps.find("img_type");
        std::string type = ps.find("type");
        int x_res = std::stoi(ps.find("x_res"));
        int y_res = std::stoi(ps.find("y_res"));
        film = new Film(filename, img_type, type, x_res, y_res);
}

void Api::BACKGROUND(Paramset<std::string, std::string> ps){
    std::string type = ps.find("type");
    std::string color(ps.find("color"));
    if(color != ""){
        Vec c(color);
        background = new Background(type, c);
    } else {
        Vec bl(ps.find("bl"));
        Vec br(ps.find("br"));
        Vec tl(ps.find("tl"));
        Vec tr(ps.find("tr"));
        std::string mapping = ps.find("mapping");
        background = new Background(type, bl, br, tl, tr, mapping);
    }
}

Camera Api::getCamera(){
    return *camera;
}

Film Api::getFilm(){
    return *film;
}

Background Api::getBackground(){
    return *background;
}

void Api::render(){
    auto w = film->width();
    auto h = film->height();
    
    camera->setHeightWidth(h, w);
    for(int j = h-1; j >= 0 ; j--){
        for(int i = 0; i < w; i++){
            
            //Ray r1 = camera->generate_ray( float(i)/float(w), float(j)/float(h) );
            // Generate ray with the Shirley method.
            Ray r2 = camera->generate_ray( i, j );
            // Print out the two rays, that must be the same (regardless of the method).
            std::cout << "Point: [" << j << "," << i <<"] Ray1: " << r2 << std::endl;

            auto color = background->sample(float(i)/float(w), float(j)/float(h));
            film->add(i, j, color);
        }
    }

    film->write_image();
}

#endif