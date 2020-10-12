#ifndef _API_
#define _API_


#include <string>
#include <memory>
#include <vector>
#include <utility>
#include "camera.hpp"
#include "film.hpp"
#include "background.hpp"
#include "paramset.hpp"
#include "vec.hpp"
#include "scene.hpp"
#include "geometricprimitive.hpp"

#include "sphere.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "flatmaterial.hpp"
#include "aggregateprimitive.hpp"
#include "integrator.hpp"

class Api
{
private:
    /* data */
    Camera* camera;

    Film* film;

    Background* background;
    //Material* mat;
    std::vector<std::shared_ptr<GeometricPrimitive>> obj_list_;
    Scene* scene;

    Integrator * integrator;

public:
    Api(/* args */);
    ~Api();

    void CAMERA(Paramset<std::string, std::string> ps);

    void FILM(Paramset<std::string, std::string> ps);

    void BACKGROUND(Paramset<std::string, std::string> ps);

    void MATERIAL(Paramset<std::string, std::string> ps);

    void OBJECTS(std::vector<std::pair<Paramset<std::string, std::string>, Paramset<std::string, std::string>>> ps);

    void INTEGRATOR(Paramset<std::string, std::string> ps);
    
    Camera getCamera();

    Film getFilm();

    Background getBackground();
    
    void render();

};

Api::Api(/* args */) : camera{nullptr}, film{nullptr}, background{nullptr}, integrator{nullptr}
{
    scene = new Scene();
}

Api::~Api(){
    if(scene)
        delete scene;
    if(camera)
        delete camera;
    if(film)
        delete film;
    if(background)
        delete background;
    if(integrator)
        delete integrator;
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

    scene->setCamera(camera);
}

void Api::FILM(Paramset<std::string, std::string> ps){
        std::string filename = ps.find("filename");
        std::string img_type = ps.find("img_type");
        std::string type = ps.find("type");
        int x_res = std::stoi(ps.find("x_res"));
        int y_res = std::stoi(ps.find("y_res"));
        film = new Film(filename, img_type, type, x_res, y_res);

        scene->setFilm(film);
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

    scene->setBackground(background);
}

// void Api::MATERIAL(Paramset<std::string, std::string> ps){
//     std::string type = ps.find("type");
//     if(type == "flat"){
//         std::string color = ps.find("color");
//         Vec c(color);
//         mat = new FlatMaterial(c);

//     } else{
//         std::cout << "TIPO DO MATERIAL NÃO COMPATIVEL\n";
//     }
// }

void Api::OBJECTS(std::vector<std::pair<Paramset<std::string, std::string>, Paramset<std::string, std::string>>> ps){
    for(std::pair<Paramset<std::string, std::string>, Paramset<std::string, std::string>> p : ps){
        std::string type_object = std::get<1>(p).find("type");
        std::string type_integrator = std::get<0>(p).find("type");

        std::shared_ptr<GeometricPrimitive> geo_pri =  std::make_shared<GeometricPrimitive>();

        //std::cout << type_integrator <<  "\n\n\n";

        if(type_object == "sphere"){
        //std::cout << "ENTROU\n\n\n";

            float radius_ = std::stof(std::get<1>(p).find("radius"));
            std::string center = std::get<1>(p).find("center");
            // Quebrar os espaços do centro e gerar o vetor centro
            std::vector<std::string> result; 
            std::istringstream iss(center); 
            for(std::string s; iss >> s; ) 
                result.push_back(s);
            Vec center_(std::stof(result[0]), std::stof(result[1]), std::stof(result[2]));
            //center_.print();
            center_.v4 = 0;
            Shape * obj_ = new Sphere(false, center_, radius_);
            geo_pri->set_shape(obj_);
        }

        if(type_integrator == "flat"){
            std::string color = std::get<0>(p).find("color");
            // Quebrar os espaços do centro e gerar o vetor centro
            std::vector<std::string> result; 
            std::istringstream iss(color); 
            for(std::string s; iss >> s; ) 
                result.push_back(s);
            Vec center_(std::stof(result[0])/255.0, std::stof(result[1])/255.0, std::stof(result[2])/255.0);
            //center_.print();
            center_.v4 = 0;
            Material * fl_ma = new FlatMaterial(center_); 
            geo_pri->set_material(fl_ma);
        }


        obj_list_.push_back(geo_pri);
    }

    scene->setObjList(obj_list_);
}

void Api::INTEGRATOR(Paramset<std::string, std::string> ps){
    std::string type =  ps.find("type");
    if(type == "flat"){
        integrator = new FlatIntegrator(type);
    } else if (type == "normal_flat") {
        integrator = new NormalIntegrator(type);
    } else if (type == "depth_map"){
        float zmin = std::stof(ps.find("zmin"));
        float zmax = std::stof(ps.find("zmax"));
        std::string nearColor_ = ps.find("near_color");
        std::vector<std::string> result1; 
        std::istringstream iss(nearColor_); 
        for(std::string s; iss >> s; ) 
            result1.push_back(s);
        Vec nearColor(std::stof(result1[0])/255.0, std::stof(result1[1])/255.0, std::stof(result1[2])/255.0);
        
        std::string farColor_ = ps.find("far_color");
        std::vector<std::string> result2; 
        std::istringstream iss1(farColor_); 
        for(std::string s; iss1 >> s; ) 
            result2.push_back(s);
        Vec farColor(std::stof(result2[0])/255.0, std::stof(result2[1])/255.0, std::stof(result2[2])/255.0);

        integrator = new DepthIntegrator(type, zmin, zmax, nearColor, farColor);
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
    
    auto w = scene->film->width();
    auto h = scene->film->height();
    
    camera->setHeightWidth(h, w);

    integrator->preprocess(this->scene, this->camera);


    for(int j = h-1; j >= 0 ; j--){
        for(int i = 0; i < w; i++){
            
            Ray ray = camera->generate_ray( i, j );
            Vec color_(0,0,0);

            // std::cout << i << " " << j << std::endl;
            // std::cout << ray << std::endl;

            if(background->getMapping() == "screen"){
                color_ = background->sample(float(i)/float(w), float(j)/float(h));
            }
            
            Color24 c = integrator->Li(ray, scene, color_);
            scene->film->add(i, j, c);
        }
    }

    scene->film->write_image();
}

#endif