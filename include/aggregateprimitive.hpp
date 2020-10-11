#ifndef _AGGREGPRIM_
#define _AGGREGPRIM_

#include "./material.hpp"

class AggregatePrimitive : public Primitive{
    public:
        Material * get_material();
        bool intersect( const Ray& r, Surfel *sf ) = 0;
		bool intersect_p( const Ray& r ) = 0;
};

Material * AggregatePrimitive::get_material(){
    return nullptr;
}


#endif