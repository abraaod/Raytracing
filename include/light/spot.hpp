#ifndef _SPOT_
#define _SPOT_

#include <string>
#include "../vec.hpp"
#include "./light.hpp"
<<<<<<< HEAD
#include "../scene.hpp"
=======
>>>>>>> 50e8242ba12a1075d0ad38d7e1608f9afd2b3372
#include "math.h"

class SpotLight : public Light{
    public:

    Vec i;
    Vec scale;
    Vec from;
    Vec to;
    float cutoff;
    float falloff;

    SpotLight(std::string type, Vec i, Vec scale, Vec from, Vec to, float cutoff, float falloff) : Light(type){
        this->i = i;
        this->scale = scale;
        this->from = from;
        this->to = to;
        this->cutoff = cos(((cutoff*M_PI)/180.0)/2.0);
        this->falloff = cos(((falloff*M_PI)/180.0)/2.0);
    }

<<<<<<< HEAD
    void preprocessLight(Scene & scene){
    }

=======
>>>>>>> 50e8242ba12a1075d0ad38d7e1608f9afd2b3372
    Vec sample_Li(const Surfel& hit /*in*/, Vec v, Vec *wi) {

        Vec lightDir = from - hit.p;
        lightDir = normalize(lightDir);

        Vec spotDir = to - from;
        spotDir = normalize(-spotDir);

        float theta = dot(lightDir, spotDir);///(magnitude(lightDir)*magnitude(spotDir));
        
        float epsilon   = falloff - cutoff;
        float intensity = std::min(1.0f, std::max(0.f, (theta - cutoff) / epsilon));
        
        *wi = i * intensity;
        return spotDir;
    }
};

#endif