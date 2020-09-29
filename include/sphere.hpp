#ifndef _SPHERE_
#define _SPHERE_-

#include "primitive.hpp"

class Sphere : public Primitive {

    public:

        Vec *center;
        float radius;

        virtual ~Sphere() = default;

        bool intersect( const Ray& r, Surfel *sf );
        
        bool intersect_p( const Ray& r );

        Sphere(Vec * center = nullptr, float radius = 0.0) : center{center}, radius{radius} {};

};

bool Sphere::intersect(const Ray& r, Surfel *sf){

    Ray ray = r;
    Vec oc = ray.getOrigin() - (*center);
    float a = (dot(ray.getDirection(), ray.getDirection()));
    float b = 2.0 * (dot(oc, ray.getDirection()));
    float c = dot(oc, oc) - (radius*radius);
    
    float discriminat = (b*b) -4 * a * b;
    if(discriminat >= 0){
        return true;
    }

    return false;
    
}

bool Sphere::intersect_p(const Ray& r){

    Ray ray = r;
    Vec oc = ray.getOrigin() - (*center);
    float a = (dot(ray.getDirection(), ray.getDirection()));
    float b = 2.0 * (dot(oc, ray.getDirection()));
    float c = dot(oc, oc) - (radius*radius);
    
    float discriminat = (b*b) -4 * a * b;
    if(discriminat >= 0){
        return true;
    }

    return false;
}

#endif