#ifndef _TRIANGLE_
#define _TRIANGLE_

#include <memory>
#include "shape.hpp"
#include "trianglemesh.hpp"
#include "vec.hpp"

using Point = Vec;


class Triangle : public Shape{
    private:
		int *v; //!< Pointer to the vertex index array stored in the triangle mesh database. 
		// After the proper triangle initialization, we may access vertices with v[0], v[1], and v[2]
		int *n; //!< Pointer to the normal index array stored in the triangle mesh database.
		// After the proper triangle initialization, we may access normals with n[0], n[1], and n[2]
		int *uv;//!< Pointer to the UV coord index array stored in the triangle mesh database.
		// After the proper triangle initialization, we may access UV coords with uv[0], uv[1], and uv[2]

		std::shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.
		bool backface_cull;            //!< Set it `true` to activate backface culling.
	
	public:
        // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
		Triangle( std::shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc=true )
			: Shape(), mesh{mesh}, backface_cull{bfc}
		{
			// This is just a shortcut to access this triangle's data stored in the mesh database.
			v  = &mesh->vertex_indices[ 3 * tri_id ];
			n  = &mesh->normal_indices[ 3 * tri_id ];
			uv = &mesh->uvcoord_indices[ 3 * tri_id ];
		}
		/// Return the triangle's bounding box.
		// Bounds3f object_bound() const;
        /// The regular intersection methods, as defined in the Shape parent class.
		bool intersect(const Ray &ray, float *thit, Surfel *isect ) const{ return true; }
		bool intersect_p( Ray &ray ) const {
			float beta;
			float gamma;
			float t_hit;


			// compute t_hit
			Point &p0 = mesh->vertices[v[0]];
			Point &p1 = mesh->vertices[v[0]];
			Point &p2 = mesh->vertices[v[0]];

			
			float a = p0.v1 - p1.v1;
			float b = p0.v2 - p1.v2;
			float c = p0.v3 - p1.v3;
			float d = p0.v1 - p2.v1;
			float e = p0.v2 - p2.v2;
			float f = p0.v3 - p2.v3;
			Vec dir = ray.getDirection();
			float g = dir.v1;
			float h = dir.v2;
			float i = dir.v3;
			Vec origem = ray.getOrigin();
			float j = p0.v1 - origem.v1;
			float k = p0.v2 - origem.v2;
			float l = p0.v3 - origem.v3;
			

			float m = a * (e*i - h*f) + b * (g*f - d*i) + c * (d*h - e*g);
			
			t_hit = - (f * (a*k - j*b) + e * (j*c - a*l) + d * (b*l- k*c))/m;

			if(t_hit < ray.tmin or t_hit > ray.tmax)
				return false;

			// compute gamma
			gamma = ( i * ((a*k) - (j*b)) + h * ((j*c) - (a*l)) + g * (b*l - k*c))/m;

			if(gamma < 0 or gamma > 1)
				return false;

			// compute betta
			beta = (j * (e*i - h*f) + k * (g*f-d*i) + l * (d*h - e*g))/m;
			
			if(beta < 0 or beta > (1-gamma))
				return false;

			return true;
		}

        /// This friend function helps us debug the triangles, if we want to.
        friend std::ostream& operator<<( std::ostream& os, const Triangle & t );

};

#endif