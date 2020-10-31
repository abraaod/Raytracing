#ifndef _INTEGRATOR_
#define _INTEGRATOR_

#include "ray.hpp"
#include "scene.hpp"
#include "surfel.hpp"
#include "light/point.hpp"

class Integrator
{
public:
    std::string type;
    Integrator(std::string type) {}
    virtual Color24 Li(Ray &ray, const Scene *scene, Color24 bkg_color) const = 0;
    virtual ~Integrator() = default;

private:
};

class FlatIntegrator : public Integrator
{
public:
    FlatIntegrator(std::string type) : Integrator(type)
    {
        this->type = type;
    }

    Color24 Li(Ray &ray, const Scene *scene, Color24 bkg_color) const
    {

        Color24 color_ = bkg_color;
        auto obj_list_ = scene->obj_list;

        Surfel sf;
        for (int k = 0; k < obj_list_.size(); k++)
        {
            if (obj_list_[k]->intersect(ray, &sf))
            {
                color_ = sf.primitive->get_material()->kd();
            }
        }

        return color_;
    }
    virtual ~FlatIntegrator() = default;
};

class NormalIntegrator : public Integrator
{
public:
    NormalIntegrator(std::string type) : Integrator(type)
    {
        this->type = type;
    }

    Color24 Li(Ray &ray, const Scene *scene, Color24 bkg_color) const
    {

        Color24 color_ = bkg_color;
        auto obj_list_ = scene->obj_list;

        Surfel sf;
        for (int k = 0; k < obj_list_.size(); k++)
        {
            if (obj_list_[k]->intersect(ray, &sf))
            {
                //sf.primitive->get_material()->kd();
                Vec normal = unique_vector(sf.n);
                Vec c(normal.v1 + 1, normal.v2 + 1, normal.v3 + 1, 0);
                c = c * 0.5;
                //c.print();
                color_ = c;
            }
        }

        return color_;
    }
    virtual ~NormalIntegrator() = default;
};

class BlinnPhongIntegrator : public Integrator
{
public:
    BlinnPhongIntegrator(std::string type) : Integrator(type)
    {
        this->type = type;
    }

    Color24 Li(Ray &ray, const Scene *scene, Color24 bkg_color) const
    {

        Color24 color_ = bkg_color;
        auto obj_list_ = scene->obj_list;
        auto lights = scene->lights;

        Surfel sf;
        for (int k = 0; k < obj_list_.size(); k++)
        {
            if (obj_list_[k]->intersect(ray, &sf))
            {
                
                // Vec light_pos{1, 3, 3};      // Point light location    (hardcoded here, for now)
                // Vec light_I{0.9, 0.9, 0.9}; // Point light Intensity   (hardcoded here, for now)
                // Vec l;                      // This is the light vector.
                // l = light_pos - sf.p;         // Determine the vector from the light to the hit point `p`.
                // l = normalize(l);
                // //l.print();

                // // Determining pixel color based on **Lambertian Shading** only.
                // Vec c {sf.n};
                // c = normalize(c);
                // //std::cout << std::max(0.f, dot(c, l)) << std::endl;
                // BlinnMaterial *fm = dynamic_cast< BlinnMaterial *>( sf.primitive->get_material() );
                // color_ = fm->kd() * light_I * std::max(0.f, dot(c, l));
                Vec c;
                Vec wi;
                BlinnMaterial *bm = dynamic_cast< BlinnMaterial *>( sf.primitive->get_material() );
                for(int i = 0;  i < lights.size(); i++){
                    
                    Vec l = lights[i]->sample_Li(sf, ray.getOrigin(), &wi);
                    Vec v = ray.getOrigin() - sf.p;
                    v = normalize(v);
                    //v = normalize(v);
                    Vec n =  normalize(sf.n);

                    
                    
                    Vec h =  (v + l)/(magnitude(v+l));// * magnitude(dir_))); 

                    
                    
                    c = c + (bm->kd() * wi * std::max(0.f, dot(n, l))) +  (bm->ks() * wi *  std::pow(std::max(0.f, dot(n, h)), bm->glossiness));
                    

                    Vec norma_p = normalize(sf.p);
                    Surfel sp;
                    
                    for (int q = 0; q < obj_list_.size(); q++){
                        for(int k = 0;  k < lights.size(); k++){
                            Ray shadow_ray(sf.p, lights[k]->from);
                            if (obj_list_[q]->intersect(shadow_ray, &sp)){
                                
                                return Vec(0,0,0);
                            }
                        }
                    }
                }

                
                if(scene->ambient != nullptr){
                    Vec la = scene->ambient->l;
                    color_ = c + (bm->ka() * la);
                } else {
                    color_ = c;
                }
                
                
                if(color_.v1 > 1.0){
                    color_.v1 = 1.0;
                }


                if(color_.v2 > 1.0){
                    color_.v2 = 1.0;
                }


                if(color_.v3 > 1.0){
                    color_.v3 = 1.0;
                }
            }
        }

        return color_;
    }
    virtual ~BlinnPhongIntegrator() = default;
};

#endif