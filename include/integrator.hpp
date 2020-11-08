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
    virtual Color24 Li(Ray &ray, const Scene *scene, Color24 bkg_color, int depth) const = 0;
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

    Color24 Li(Ray &ray, const Scene *scene, Color24 bkg_color, int depth) const
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

    Color24 Li(Ray &ray, const Scene *scene, Color24 bkg_color, int depth) const
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
    int max_depth;

    BlinnPhongIntegrator(std::string type, int depth) : Integrator(type)
    {
        this->type = type;
        this->max_depth = depth;
    }

    Color24 Li(Ray &ray, const Scene *scene, Color24 bkg_color, int depth) const
    {
        //std::cout << depth << std::endl;

        Color24 color_ = bkg_color;
        auto obj_list_ = scene->obj_list;
        auto lights = scene->lights;

        Surfel sf;
        for (int k = 0; k < obj_list_.size(); k++)
        {
            if (obj_list_[k]->intersect(ray, &sf))
            {

                Vec c;
                Vec wi;
                BlinnMaterial *bm = dynamic_cast<BlinnMaterial *>(sf.primitive->get_material());
                Vec n = normalize(sf.n);

                for (int i = 0; i < lights.size(); i++)
                {

                    Vec l = lights[i]->sample_Li(sf, ray.getOrigin(), &wi);

                    Ray shadow_ray;
                    if (lights[i]->type == "directional")
                    {
                        shadow_ray = Ray(sf.p, l);
                    }
                    else
                    {
                        float dis = distance(sf.p, lights[i]->from);
                        shadow_ray = Ray(sf.p, l, 0.0, dis);

                        Vec d_ = sf.p - lights[i]->from;
                    }

                    bool hittou = false;

                    for (int z = 0; z < obj_list_.size(); z++)
                    {
                        if (z != k)
                        {
                            hittou = obj_list_[z]->intersect_p(shadow_ray);
                        }
                        if (hittou)
                        {
                            break;
                        }
                    }

                    if (!hittou)
                    {

                        Vec v = ray.getOrigin() - sf.p;
                        v = normalize(v);

                        Vec h = (v + l) / (magnitude(v + l)); // * magnitude(dir_)));

                        c = c + (bm->kd() * wi * std::max(0.f, dot(n, l))) + (bm->ks() * wi * std::pow(std::max(0.f, dot(n, h)), bm->glossiness));
                    }

                    // if(cosTheta < 0){
                    //     std::cout << cosTheta << std::endl;
                    //     break;
                    //     // return Vec(0.0, 0.0, 0.0);
                    // }
                    // float grau = cosTheta * (180 / M_PI);
                    // std::cout << grau << std::endl;
                    // if (grau >= 90.0 || grau <= -90.0)
                    //     return Vec(0.0, 0.0, 0.0);
                }

                if (scene->ambient != nullptr)
                {
                    Vec la = scene->ambient->l;
                    color_ = c + (bm->ka() * la);
                }
                else
                {
                    color_ = c;
                }

                if (color_.v1 > 1.0)
                {
                    color_.v1 = 1.0;
                }

                if (color_.v2 > 1.0)
                {
                    color_.v2 = 1.0;
                }

                if (color_.v3 > 1.0)
                {
                    color_.v3 = 1.0;
                }

                if (depth < max_depth)
                {
                    Ray reflected_ray = Ray(sf.p, ray.getDirection() - n * (2 * (dot(ray.getDirection(), n))));
                    color_ = color_ + bm->km() * Li(reflected_ray, scene, color_, depth + 1);
                }
            } else{
                float cosTheta = dot(normalize(sf.n), -ray.getDirection());
                // std::cout << cosTheta << std::endl;
                if (cosTheta < 0.0 and ray.tmin < 0.0 and ray.tmax > 0.0)
                {
                    return Vec(0, 0, 0);
                }
            }
        }
        //color_.print();
        return color_;
    }
    virtual ~BlinnPhongIntegrator() = default;
};

#endif