#ifndef _SURFEL_
#define _SURFEL_


#include "vec.hpp"
#include "point2.hpp"
#include "primitive.hpp"
#include "bounds3.hpp"

class Primitive;

class Surfel{
	public:
		Vec p;        //!< Contact point.
		Vec n;       //!< The surface normal.
		Vec wo;      //!< Outgoing direction of light, which is -ray.
		float time;     //!< Time of contact.
		Point2 uv;     //!< Parametric coordinate (u,v) of the hit surface.
		Primitive *primitive=nullptr; //!< Pointer to the primitive.
		std::shared_ptr<Bounds3> bound;

		Surfel() {}
		Surfel( const Vec&p,const Vec&n, const Vec&wo, float time, 
			const Point2& uv, Primitive *pri )
			: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}
            {/* empty */};
};

#endif