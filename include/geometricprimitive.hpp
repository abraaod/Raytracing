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

class GeometricPrimitive : public Primitive
{
private:
    std::shared_ptr<Shape> shape;

public:
    GeometricPrimitive() : Primitive(){};
    //GeometricPrimitive(Shape * s, Material * m);
    // Bounds3f world_bounds();
    bool intersect(Ray &r, Surfel *s);
    bool intersect_p(Ray &r);
    std::shared_ptr<Material> get_material() const;
    void set_material(std::shared_ptr<Material> m);
    Bounds3 world_bounds(){
        shape->world_bounds();
    }
    void printCenter()
    {
        std::cout << "_-----------------";
        shape->printCenter();
    }
    std::shared_ptr<Shape> get_Shape();
    Bounds3 union_geo(const Bounds3 &b, const Point &p)
    {
        return Bounds3(Point(std::min(b.pMin.v1, p.v1),
                             std::min(b.pMin.v2, p.v2),
                             std::min(b.pMin.v3, p.v3)),
                       Point(std::max(b.pMax.v1, p.v1),
                             std::max(b.pMax.v2, p.v2),
                             std::max(b.pMax.v3, p.v3)));
    }

    //Realiza a união de 2 bounds
    Bounds3 unionBounds(const Bounds3 &b1, const Bounds3 &b2)
    {
        return Bounds3(Point(std::min(b1.pMin.v1, b2.pMin.v1),
                             std::min(b1.pMin.v2, b2.pMin.v2),
                             std::min(b1.pMin.v3, b2.pMin.v3)),
                       Point(std::max(b1.pMax.v1, b2.pMax.v1),
                             std::max(b1.pMax.v2, b2.pMax.v2),
                             std::max(b1.pMax.v3, b2.pMax.v3)));
    }

    Bounds3 intersectBounds(const Bounds3 &b1, const Bounds3 &b2)
    {
        return Bounds3(Point(std::max(b1.pMin.v1, b2.pMin.v1),
                             std::max(b1.pMin.v2, b2.pMin.v2),
                             std::max(b1.pMin.v3, b2.pMin.v3)),
                       Point3(std::min(b1.pMax.v1, b2.pMax.v1),
                              std::min(b1.pMax.v2, b2.pMax.v2),
                              std::min(b1.pMax.v3, b2.pMax.v3)));
    }

    bool overlapsBounds(const Bounds3 &b1, const Bounds3 &b2)
    {
        bool x = (b1.pMax.v1 >= b2.pMin.v1) && (b1.pMin.v1 <= b2.pMax.v1);
        bool y = (b1.pMax.v2 >= b2.pMin.v2) && (b1.pMin.v2 <= b2.pMax.v2);
        bool z = (b1.pMax.v3 >= b2.pMin.v3) && (b1.pMin.v3 <= b2.pMax.v3);
        return (x && y && z);
    }

    bool insideBounds(const Point &p, const Bounds3 &b)
    {
        return (p.v1 >= b.pMin.v1 && p.v1 <= b.pMax.v1 &&
                p.v2 >= b.pMin.v2 && p.v2 <= b.pMax.v2 &&
                p.v3 >= b.pMin.v3 && p.v3 <= b.pMax.v3);
    }

    Bounds3 expandBounds( Bounds3 &b, float delta)
    {
        return Bounds3(b.pMin - Point(delta, delta, delta),
                       b.pMax + Point(delta, delta, delta));
    }

    void set_shape(std::shared_ptr<Shape> s);

    ~GeometricPrimitive() = default;
};

// GeometricPrimitive::GeometricPrimitive(Shape * s, Material * m){
//     this->shape = s;
//     this->material = m;
// }

// Bounds3f GeometricPrimitive::world_bounds(){

// }

bool GeometricPrimitive::intersect(Ray &r, Surfel *s)
{
    float thit;

    // shape->printCenter();
    // Triangle *t = dynamic_cast<Triangle*>(shape);
    // std::cout << *t << std::endl;
    if (shape->intersect(r, &thit, s))
    {
        if (thit < r.tmax and thit > 0.0)
        {
            r.tmax = thit;
            s->primitive = this;
            //std::cout << "Bateu" << std::endl;
            return true;
        }
    }

    return false;
}

bool GeometricPrimitive::intersect_p(Ray &ray)
{
    if (shape->intersect_p(ray))
    {
        return true;
    }
    return false;
}

std::shared_ptr<Material> GeometricPrimitive::get_material() const
{
    return material;
}

void GeometricPrimitive::set_material(std::shared_ptr<Material> m)
{
    this->material = m;
}

std::shared_ptr<Shape> GeometricPrimitive::get_Shape()
{
    return shape;
}

void GeometricPrimitive::set_shape(std::shared_ptr<Shape> s)
{
    this->shape = s;
}
#endif
