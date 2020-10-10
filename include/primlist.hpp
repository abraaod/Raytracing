#ifndef _PRIMLIST_
#define _PRIMLIST_

#include <vector>
#include "aggregateprimitive.hpp"
#include "primitive.hpp"

class PrimList: public AggregatePrimitive{
    private:
        std::vector<Primitive *> primitives;

    public:
        PrimList(std::vector<Primitive *> prim);
        // Bounds3f world_bounds();
        bool intersect( const Ray& r, Surfel *sf ) = 0;
		bool intersect_p( const Ray& r ) = 0;

};

PrimList::PrimList(std::vector<Primitive *> prim){
    this->primitives = prim;
}

bool PrimList::intersect( const Ray & r, Surfel * sf){
    // Implement when is requested  
    return false;
}

bool PrimList::intersect_p( const Ray & r ){
    // Implement when requested
    return false;
}

#ifndef