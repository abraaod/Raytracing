#ifndef _PRIMITIVE_
#define _PRIMITIVE_

#include <memory>
#include "ray.hpp"
#include "surfel.hpp"
#include "material.hpp"

class Primitive {
	public:
		Material * material;
		virtual bool intersect( const Ray& r, Surfel *sf ) {};
		// Simpler & faster version of intersection that only return true/false.
		// It does not compute the hit point information.
		virtual bool intersect_p( const Ray& r ) {};
		Primitive() {};
		virtual Material * get_material() const {return material;}
		virtual ~Primitive() = default;
		//virtual const Material * get_material(void) const = { return material; }
	private:
		//std::shared_ptr<Material> material;
};
#endif