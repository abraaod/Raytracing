#ifndef _INTEGRATOR_
#define _INTEGRATOR_

#include "ray.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "surfel.hpp"
#include "vec.hpp"
#include <limits>
#include <vector>

class Integrator {
    public:
        std::string type;
        Integrator(std::string type) {}
        virtual Color24 Li(Ray& ray, const Scene *scene, Color24 bkg_color) const = 0;
        virtual ~Integrator() = default;
        void preprocess( const Scene * scene, Camera *cam ){}
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

class DepthIntegrator : public Integrator{
    private:
    float z_min;
    float z_max;
    Color24 near_color;
    Color24 far_color;
    std::vector<std::vector<double>> depth_buffer;

    public:
    DepthIntegrator(std::string type, float z_min, float z_max, Color24 near_color, Color24 far_color): Integrator(type){
        this->type = type;
        this->z_min = z_min;
        this->z_max = z_max;
        this->near_color = near_color;
        this->far_color = far_color;
    }

    DepthIntegrator(std::string type, float z_min, float z_max): Integrator(type){
        this->type = type;
        this->z_min = z_min;
        this->z_max = z_max;
        this->near_color = Color24(1,1,1);
        this->far_color = Color24(0,0,0);
    }

    Color24 Li(Ray& ray, const Scene *scene, Color24 bkg_color) const{

        Color24 color_ = bkg_color;
        auto obj_list_ = scene->obj_list;

        Surfel sf;
        for(int k = 0; k < obj_list_.size(); k++){

                float z_v1 = sf.p.v1 - z_min / (z_max - z_min);
                float z_v2 = sf.p.v2 - z_min / (z_max - z_min);
                float z_v3 = sf.p.v3 - z_min / (z_max - z_min);
                
                Vec z(z_v1, z_v2, z_v3);
                // std::cout << "----------------------------\n";
                // z.print();

                // std::cout << "COLORS = FC - " << far_color.v2;

                Vec black(0,0,0);
                Vec white(1,1,1);
                
                float v1_ = black.v1*(1-z_v1)*(1-z_v2) + black.v1*z_v1*(1-z_v2) + white.v1*(1-z_v1)*z_v2 + white.v1*z_v1*z_v2;
                float v2_ = black.v2*(1-z_v1)*(1-z_v2) + black.v2*z_v1*(1-z_v2) + white.v2*(1-z_v1)*z_v2 + white.v2*z_v1*z_v2;
                float v3_ = black.v3*(1-z_v1)*(1-z_v2) + black.v3*z_v1*(1-z_v2) + white.v3*(1-z_v1)*z_v2 + white.v3*z_v1*z_v2;

                Vec c(v1_, v2_, v3_);

                color_ = c;
            // if(obj_list_[k]->intersect(ray, &sf)){
                
            //     float z_v1 = sf.p.v1 - z_min / (z_max - z_min);
            //     float z_v2 = sf.p.v2 - z_min / (z_max - z_min);
            //     float z_v3 = sf.p.v3 - z_min / (z_max - z_min);
                
            //     Vec z(z_v1, z_v2, z_v3);

                

            //     Vec black(0,0,0);
            //     Vec white(1,1,1);
                
            //     float v1_ = black.v1*(1-z_v1)*(1-z_v2) + black.v1*z_v1*(1-z_v2) + white.v1*(1-z_v1)*z_v2 + white.v1*z_v1*z_v2;
            //     float v2_ = black.v2*(1-z_v1)*(1-z_v2) + black.v2*z_v1*(1-z_v2) + white.v2*(1-z_v1)*z_v2 + white.v2*z_v1*z_v2;
            //     float v3_ = black.v3*(1-z_v1)*(1-z_v2) + black.v3*z_v1*(1-z_v2) + white.v3*(1-z_v1)*z_v2 + white.v3*z_v1*z_v2;

            //     Vec c(v1_, v2_, v3_);

            //     color_ = c;
            // }
        }

        return color_;
    }

    void preprocess( const Scene * scene, Camera *cam ){
        for(int j = scene->film->height()-1; j >= 0 ; j--){
            std::vector<double> aux;
            for(int i = 0; i < scene->film->width(); i++){
                double a = std::numeric_limits<double>::max();
                aux.push_back(a);
            }
            depth_buffer.push_back(aux);
        }

        for(int j = scene->film->height()-1; j >= 0 ; j--){
            for(int i = 0; i < scene->film->width(); i++){
                Ray ray = cam->generate_ray( i, j );
                
                auto obj_list_ = scene->obj_list;

                Surfel sf;
                for(int k = 0; k < obj_list_.size(); k++){
                    if(obj_list_[k]->intersect(ray, &sf)){
                        if( ray.tmax < depth_buffer[i][j])
                            depth_buffer[i][j] = ray.tmax;
                    }
                }
            }
        }
    }

    virtual ~DepthIntegrator() = default;
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