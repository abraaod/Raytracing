#ifndef _GEOPRIM_
#define _GEOPRIM_

#include <memory>
#include "./shape.hpp"
#include "./material.hpp"
#include "./vec.hpp"

// using Bounds3f = Vec;

class GeometricPrimitive : public Primitive{
    private:
    Shape * shape;
    Material * material;

    public:
    GeometricPrimitive() : Primitive() {};
    GeometricPrimitive(Shape * s, Material * m);
    // Bounds3f world_bounds();
    bool intersect(Ray r, Surfel s);
    bool intersect_p(Ray r);
    Material * get_material();
    void set_material(Material * m);
    Shape * get_Shape();
    void set_shape(Shape * s);
    ~GeometricPrimitive(){
        if(shape)
            delete shape;
        if(material)
            delete material;
    }
};

GeometricPrimitive::GeometricPrimitive(Shape * s, Material * m){
    this->shape = s;
    this->material = m;
}

// Bounds3f GeometricPrimitive::world_bounds(){
    
// }

bool GeometricPrimitive::intersect(Ray r, Surfel s){
    return false;   
}

bool GeometricPrimitive::intersect_p(Ray r){
    return false;
}

Material * GeometricPrimitive::get_material(){
    return material;
}

void GeometricPrimitive::set_material(Material * m){
    this->material = m;
}

Shape * GeometricPrimitive::get_Shape(){
    return shape;
}

void GeometricPrimitive::set_shape(Shape * s){
    this->shape = s;
}
#endif