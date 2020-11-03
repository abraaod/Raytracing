#ifndef _SPHERE_
#define _SPHERE_

#include <cmath>
#include "shape.hpp"

class Sphere : public Shape {

    public:
        Sphere(bool flip_n, Vec center, float radius);
        

        Vec center;
        float radius;
        

        bool intersect( Ray& r,float * t_hit, Surfel *sf );
        bool intersect_p( Ray & r );

        void printCenter(){
            center.print();
        }

        // Sphere(Vec * center = nullptr, float radius = 0.0) : center{center}, radius{radius} {};
        virtual ~Sphere() = default;

};

Sphere::Sphere(bool flip_n, Vec center, float radius){
    this->center= center;
    this->radius = radius;
}

bool Sphere::intersect( Ray& r,float * t_hit, Surfel *sf ){
    Ray ray = r;
    Vec oc = ray.getOrigin() - (center);
    float a = (dot(ray.getDirection(), ray.getDirection()));
    float b = 2 * (dot(oc, ray.getDirection()));
    float c = dot(oc, oc) - (radius*radius);

    float discriminat = (b*b) - 4* a * c;
    if((discriminat) >= 0){

        // auto t = (-b - sqrt(discriminat)) / (a);

        // if(t > r.tmin and t < r.tmax){
        //     sf->p = r(t);
        //     sf->n = sf->p - center;
        //     *t_hit = t;
        //     sf->primitive = dynamic_cast<Primitive *>(this);
        // }

        // t = (-b + sqrt(discriminat)) / (a);

        // if(t > r.tmin and t < r.tmax){
        //     sf->p = r(t);
        //     sf->n = sf->p - center;
        //     *t_hit = t;
        //     sf->primitive = dynamic_cast<Primitive *>(this);
        // }

        float tmin =  (-b -sqrt(discriminat))/ (2*a);
        float tmax =  (-b +sqrt(discriminat))/ (2*a);
        if(tmin < tmax){
            *t_hit = tmin;
            sf->p = r.point_at_parameter(tmin);
            sf->n = sf->p - center;
        } else {
            *t_hit = tmax;
            sf->p = r.point_at_parameter(tmax);
            sf->n = sf->p - center;
        }
        return true;
    }

    return false;
    
}

bool Sphere::intersect_p(Ray& r){
    Ray ray = r;
    Vec oc = ray.getOrigin() - (center);
    float a = (dot(ray.getDirection(), ray.getDirection()));
    float b = 2.0*(dot(oc, ray.getDirection()));
    float c = dot(oc, oc) - (radius*radius);

    float discriminat = ( b * b ) - 4.0 * a * c;
    
    if(discriminat > 0){
        auto t  = (-b - sqrt(discriminat)) / (2.0*a);
        // float tmin =  (-b -sqrt(discriminat))/ (2*a);
        // float tmax =  (-b +sqrt(discriminat))/ (2*a);
        
        
        if(t > r.tmin and t < r.tmax){
            return true;
        }

        t = (-b + sqrt(discriminat)) / (2.0*a);

        if(t > r.tmin and t < r.tmax){
            return true;
        }
        
        // if(tmin  < tmax){
        //     *t_hit = tmin;
        // } else {
        //     *t_hit = tmax;
        // }
        // return true;
    }

    return false;
}

#endif