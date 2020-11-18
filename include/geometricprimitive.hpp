#ifndef _GEOPRIM_
#define _GEOPRIM_

#include <memory>
#include "./shape.hpp"
#include "./material.hpp"
#include "./vec.hpp"
#include "./bounds3.hpp"
#include "./triangle.hpp"
#include "primitive.hpp"

using Point = Vec;

class GeometricPrimitive : public Primitive{
    private:
    
    std::shared_ptr<Shape> shape;

    public:
    GeometricPrimitive() : Primitive() {};
    //GeometricPrimitive(Shape * s, Material * m);
    // Bounds3f world_bounds();
    bool intersect(Ray& r, Surfel *s);
    bool intersect_p(Ray& r);
    std::shared_ptr<Material> get_material() const;
    void set_material(std::shared_ptr<Material> m);
    void printCenter(){
        std::cout << "_-----------------";
        shape->printCenter();
    }
    std::shared_ptr<Shape> get_Shape();
    
    // Union(const Bounds3 )
    void set_shape(std::shared_ptr<Shape>  s);

    ~GeometricPrimitive() = default;
};

// GeometricPrimitive::GeometricPrimitive(Shape * s, Material * m){
//     this->shape = s;
//     this->material = m;
// }

// Bounds3f GeometricPrimitive::world_bounds(){
    
// }

bool GeometricPrimitive::intersect(Ray& r, Surfel * s){
    float thit;

    // shape->printCenter();
    // Triangle *t = dynamic_cast<Triangle*>(shape);
    // std::cout << *t << std::endl;
    
    if(shape->intersect(r, &thit, s)){
        if(thit < r.tmax and thit > 0.0){
            r.tmax = thit;
            s->primitive = this;
            return true;
        }
    }
    
    return false;   
}

bool GeometricPrimitive::intersect_p(Ray& ray){
    if(shape->intersect_p(ray)){
            return true;
    }
    return false;
}

std::shared_ptr<Material> GeometricPrimitive::get_material() const{
    return material;
}

void GeometricPrimitive::set_material(std::shared_ptr<Material> m){
    this->material = m;
}

std::shared_ptr<Shape> GeometricPrimitive::get_Shape(){
    return shape;
}

void GeometricPrimitive::set_shape(std::shared_ptr<Shape> s){
    this->shape = s;
}
#endif