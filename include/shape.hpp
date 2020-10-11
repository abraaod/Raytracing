#ifndef _SHAPE_
#define _SHAPE_

#include <memory>
#include "./vec.hpp"
#include "./geometricprimitive.hpp"
#include "./material.hpp"


// using Bounds3f = Vec;

class Shape  {
    public:
    // bool flip_normals;

    // Shape(bool flip_n);
    // Bounds3f world_bounds();
    virtual bool intersect(Ray& r, float * t_hit, Surfel * s) = 0;
    virtual bool intersect_p(Ray & r) = 0;
    virtual void printCenter() = 0;
};

// Shape::Shape(bool flip_n){
//     this->flip_normals = flip_n;
// }

// bool Shape::intersect(Ray r, float * t_hit, Surfel * s){
//     // Every extended class will have to implement
//     return false;
// }

// bool Shape::intersect_p(Ray r){
//     std::cout << "entrou aq\n";
//     // Every extended class will have to implement
//     return false;
// }

#endif