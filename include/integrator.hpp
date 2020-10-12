#ifndef _INTEGRATOR_
#define _INTEGRATOR_

#include "ray.hpp"
#include "scene.hpp"
#include "surfel.hpp"

class Integrator {
    public:
        std::string type;
        Integrator(std::string type) {}
        virtual Color24 Li(Ray& ray, const Scene *scene, Color24 bkg_color) const = 0;
        virtual ~Integrator() = default;
    private:
};

class FlatIntegrator : public Integrator{
    public :
        FlatIntegrator(std::string type) : Integrator(type) {
            this->type = type;
        }
    
        Color24 Li(Ray& ray, const Scene * scene, Color24 bkg_color) const {
            
            Color24 color_ = bkg_color;
            auto obj_list_ = scene->obj_list;

            Surfel sf;
            for(int k = 0; k < obj_list_.size(); k++){
                if(obj_list_[k]->intersect(ray, &sf)){
                    //sf.primitive->get_material()->kd();
                    color_ = sf.primitive->get_material()->kd();
                }
            }

            return color_;
        }
        virtual ~FlatIntegrator() = default;
};

class NormalIntegrator : public Integrator{
    public :
        NormalIntegrator(std::string type) : Integrator(type) {
            this->type = type;
        }
    
        Color24 Li(Ray& ray, const Scene * scene, Color24 bkg_color) const {
            
            Color24 color_ = bkg_color;
            auto obj_list_ = scene->obj_list;

            Surfel sf;
            for(int k = 0; k < obj_list_.size(); k++){
                if(obj_list_[k]->intersect(ray, &sf)){
                    //sf.primitive->get_material()->kd();
                    Vec normal = unique_vector(sf.n);
                    Vec c(normal.v1+1, normal.v2+1, normal.v3 +1, 0);
                    c = c * 0.5;
                    //c.print();
                    color_ = c;
                }
            }

            return color_;
        }
        virtual ~NormalIntegrator() = default;
};

#endif