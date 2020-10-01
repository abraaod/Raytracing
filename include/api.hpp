#ifndef _API_
#define _API_


#include <string>
#include <memory>
#include <vector>
#include "camera.hpp"
#include "film.hpp"
#include "background.hpp"
#include "paramset.hpp"
#include "vec.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "flatmaterial.hpp"

class Api
{
private:
    /* data */
    Camera* camera;

    Film* film;

    Background* background;
    Material* mat;
    std::shared_ptr<Primitive> obj_list;
    std::vector<std::shared_ptr<Primitive>> obj_list_;
    Scene* scene;

public:
    Api(/* args */);
    ~Api();

    void CAMERA(Paramset<std::string, std::string> ps);

    void FILM(Paramset<std::string, std::string> ps);

    void BACKGROUND(Paramset<std::string, std::string> ps);

    void MATERIAL(Paramset<std::string, std::string> ps);

    void OBJECTS(std::vector<Paramset<std::string, std::string>> ps);

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

void Api::MATERIAL(Paramset<std::string, std::string> ps){
    std::string type = ps.find("type");
    if(type == "flat"){
        std::string color = ps.find("color");
        Vec c(color);
        mat = new FlatMaterial(c);

    } else{
        std::cout << "TIPO DO MATERIAL NÃO COMPATIVEL\n";
    }
}

void Api::OBJECTS(std::vector<Paramset<std::string, std::string>> ps){
    // for(int i = 0; i < ps.size(); i++){
    //     std::string type = ps[i].find("type");
    //     if(type == "sphere"){
    //         float radius_ = std::stof(ps[i].find("radius"));
    //         std::string center = ps[i].find("center");
    //         Vec center_(center);
    //         center_.v4 = 0;
    //         Primitive * obj = new Sphere(&center_, radius_);
    //         obj_list.push_back(obj);
    //     }
    // }
    for(Paramset<std::string, std::string> p : ps){
        std::string type = p.find("type");
        if(type == "sphere"){
            float radius_ = std::stof(p.find("radius"));
            std::string center = p.find("center");
            // Quebrar os espaços do centro e gerar o vetor centro
            std::vector<std::string> result; 
            std::istringstream iss(center); 
            for(std::string s; iss >> s; ) 
                result.push_back(s);
            Vec center_(std::stof(result[0]), std::stof(result[1]), std::stof(result[2]));
            center_.print();
            center_.v4 = 0;
            obj_list = std::make_shared<Sphere>(center_, radius_);
            obj_list_.push_back(obj_list);
        }
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
            
            Ray ray = camera->generate_ray( i, j );
            Vec color_(0,0,0);

            if(background->getMapping() == "screen"){
                color_ = background->sample(float(i)/float(w), float(j)/float(h));
            }
            
            if(obj_list_[0]->intersect_p(ray)){
                color_ = Vec(1,0,0);
            }

            if(obj_list_[1]->intersect_p(ray)){
                color_ = Vec(1,0,0);
            }

            if(obj_list_[2]->intersect_p(ray))
                color_ = Vec(1,0,0);

            // for(auto obj : obj_list_){
            //     if(obj->intersect_p(ray))
            //         color_ = Vec(255,0,0);
            // }

            film->add(i, j, color_);
        }
    }

    film->write_image();
}

#endif