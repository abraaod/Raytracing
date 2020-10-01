#ifndef _SCENE_
#define _SCENE_

#include <vector>
#include "primitive.hpp"
#include "camera.hpp"
#include "film.hpp"
#include "background.hpp"

class Scene{
    public:
        Camera * camera;
        Background * background;
        std::vector<Primitive> * obj_list;

    private:
        Scene(){}
        Scene(Camera * cam, Background * bg, std::vector<Primitive> * obj_list);
        ~Scene();
        void setCamera(Camera * camera);
        void setBackground(Background * bg);
        void setObjList(std::vector<Primitive> * obj_list);

};

Scene::Scene(Camera * cam, Background * bg, std::vector<Primitive> * obj_list){
    this->camera = cam;
    this->background = bg;
    this->obj_list = obj_list;
}

Scene::~Scene(){
    if(camera)
        delete camera;
    if(background)
        delete background;
}

void Scene::setCamera(Camera * camera){
    this->camera = camera;
}

void Scene::setBackground(Background * bg){
    this->background = bg;
}

void Scene::setObjList(std::vector<Primitive> * obj_list){
    this->obj_list = obj_list;
}

#endif