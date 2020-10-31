#ifndef _SCENE_
#define _SCENE_

#include <vector>
#include "primitive.hpp"
#include "camera.hpp"
#include "film.hpp"
#include "background.hpp"
#include "geometricprimitive.hpp"
#include "light/light.hpp"
#include "light/ambient.hpp"
#include "bounds3.hpp"

class Scene{
    public:
        Camera * camera;
        Background * background;
        Film * film;
        std::vector<std::shared_ptr<GeometricPrimitive>> obj_list;
        std::vector<std::shared_ptr<Light>> lights;
        AmbientLight * ambient = nullptr;
        Bounds3 worldBound;
        //std::vector<GeometricPrimitive> * obj_list;
        Scene() {}
        Scene(Camera * cam, Background * bg, Film * film, std::vector<std::shared_ptr<GeometricPrimitive>> obj_list);
        ~Scene() = default; 
        void setCamera(Camera * camera);
        void setBackground(Background * bg);
        void setFilm(Film * film);
        void setObjList(std::vector<std::shared_ptr<GeometricPrimitive>> obj_list);
        void setLight(std::vector<std::shared_ptr<Light>> lights);
        Bounds3 & getWorldBound() { return worldBound;}
    private:
        

};

Scene::Scene(Camera * cam, Background * bg, Film * film, std::vector<std::shared_ptr<GeometricPrimitive>> obj_list){
    this->camera = cam;
    this->background = bg;
    this->film = film;
    this->obj_list = obj_list;
}

void Scene::setCamera(Camera * camera){
    this->camera = camera;
}

void Scene::setBackground(Background * bg){
    this->background = bg;
}

void Scene::setFilm(Film * film){
    this->film = film;
}

void Scene::setObjList(std::vector<std::shared_ptr<GeometricPrimitive>> obj_list){
    this->obj_list = obj_list;
}

void Scene::setLight(std::vector<std::shared_ptr<Light>> lights){
    this->lights = lights;

    Scene s = *this;

    for(auto light : lights){
        light->preprocessLight(s);
    }
}

#endif