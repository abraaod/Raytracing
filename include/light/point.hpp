#ifndef _POINT_
#define _POINT_

#include <string>
#include "../vec.hpp"
#include "./light.hpp"

class PointLight : public Light{
    public:

    Vec i;
    Vec scale;
    Vec from;

    PointLight(std::string type, Vec i, Vec scale, Vec from) : Light(type){
        this->type = type;
        this->i = i;
        this->scale = scale;
        this->from = from;
    }

    Vec sample_Li(const Surfel& hit /*in*/, Vec v) {

        Vec dir = from - hit.p;
        dir = normalize(dir);

        Vec dir_ = v - hit.p;
        dir_ = normalize(dir_);
        //v = normalize(v);
        Vec n =  normalize(hit.n);
        Vec h =  (dir_ + dir)/(magnitude(dir_+dir));

        BlinnMaterial *bm = dynamic_cast< BlinnMaterial *>( hit.primitive->get_material());
        Vec c = (bm->kd() * i * std::max(0.f, dot(n, dir))) +  (bm->ks() * i *  std::pow(std::max(0.f, dot(n, h)), bm->glossiness));

        return c;

    }
    
};

#endif