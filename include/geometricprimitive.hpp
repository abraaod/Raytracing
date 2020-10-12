#ifndef _GEOPRIM_
#define _GEOPRIM_

#include <memory>
#include "./shape.hpp"
#include "./material.hpp"
#include "./vec.hpp"
#include "primitive.hpp"
// using Bounds3f = Vec;

class GeometricPrimitive : public Primitive{
    private:
    Shape * shape;

    public:
    GeometricPrimitive() : Primitive() {};
    GeometricPrimitive(Shape * s, Material * m);
    // Bounds3f world_bounds();
    bool intersect(Ray& r, Surfel *s);
    bool intersect_p(Ray& r);
    Material * get_material() const;
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

bool GeometricPrimitive::intersect(Ray& r, Surfel * s){
    float thit;
    if(shape->intersect(r, &thit, s)){
        if(thit < r.tmax){
            r.tmax = thit;
            s->primitive = this;
            return true;
        }
    }
    return false;   
}

bool GeometricPrimitive::intersect_p(Ray& r){
    return false;
}

Material * GeometricPrimitive::get_material() const{
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