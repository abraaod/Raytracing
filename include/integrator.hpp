#ifndef _INTEGRATOR_
#define _INTEGRATOR_

#include "ray.hpp"
#include "scene.hpp"
#include "surfel.hpp"

class Integrator {
    public:
        std::string type;
        Integrator(std::string type) {}
        virtual void print() {}
        virtual Color24 Li( const Ray& ray, const Scene *scene, Color24 bkg_color) const = 0;
        virtual ~Integrator() = default;
    private:
};

class FlatIntegrator : public Integrator{
    public :
        FlatIntegrator(std::string type) : Integrator(type) {
            this->type = type;
        }
        void print(){
            std::cout << "Teste" << std::endl;
        }
        Color24 Li( const Ray& ray, const Scene * scene, Color24 bkg_color) const {
            
            Color24 color_ = bkg_color;
            auto obj_list_ = scene->obj_list;

            //Surfel sf;
            for(int k = 0; k < obj_list_.size(); k++){
                if(obj_list_[k]->get_Shape()->intersect_p(ray)){
                    color_ = obj_list_[k]->get_material()->kd();
                }
            }

            return color_;
        }
        virtual ~FlatIntegrator() = default;
};

#endif