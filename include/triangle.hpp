#ifndef _TRIANGLE_
#define _TRIANGLE_

#include <memory>
#include "shape.hpp"
#include "paramset.hpp"
#include "../lib/tiny_obj_loader.h"
#include <bits/stdc++.h>
#include "bounds3.hpp"

/// This struct implements an indexd triangle mesh database.
struct TriangleMesh
{
	int n_triangles; //!< # of triangles in the mesh.
	// The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
	std::vector<int> vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
	std::vector<int> normal_indices;  //!< The list of indices to the normals list, for each individual triangle.
	std::vector<int> uvcoord_indices; //!< The list of indices to the UV coord list, for each individual triangle.

	// Here comes the data associated with each vertex. WARNING: The length of the vectors below may differ.
	std::vector<Vec> vertices;	  //!< The 3D geometric coordinates
	std::vector<Vec> normals;	  //!< The 3D normals.
	std::vector<Point2> uvcoords; //!< The 2D texture coordinates.

	// Regular constructor (almost all data is imported via move semantics).
	TriangleMesh(){/*empty*/};
	/// Copy constructor disabled
	TriangleMesh(const TriangleMesh &) = delete;
	/// Assign operator disabled.
	TriangleMesh &operator=(const TriangleMesh &) = delete;
	/// Move constructor.
	TriangleMesh(TriangleMesh &&other) = delete;
};

/// Represents a single triangle.
class Triangle : public Shape
{
private:
	int *v; //!< Pointer to the vertex index array stored in the triangle mesh database.
	// After the proper triangle initialization, we may access vertices with v[0], v[1], and v[2]
	int *n; //!< Pointer to the normal index array stored in the triangle mesh database.
	// After the proper triangle initialization, we may access normals with n[0], n[1], and n[2]
	int *uv; //!< Pointer to the UV coord index array stored in the triangle mesh database.
	// After the proper triangle initialization, we may access UV coords with uv[0], uv[1], and uv[2]

	std::shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.
	bool backface_cull;					//!< Set it `true` to activate backface culling.

public:
	// The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
	Triangle(std::shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc = true)
		: Shape(), mesh{mesh}, backface_cull{bfc}
	{
		// This is just a shortcut to access this triangle's data stored in the mesh database.
		v = &mesh->vertex_indices[3 * tri_id];
		n = &mesh->normal_indices[3 * tri_id];
		uv = &mesh->uvcoord_indices[3 * tri_id];
	}
	/// Return the triangle's bounding box.
	Bounds3 world_bounds(){
		Point p0 = mesh->vertices[v[0]];
		Point p1 = mesh->vertices[v[1]];
		Point p2 = mesh->vertices[v[2]];
		return unionBounds(Bounds3(p0, p1), p2);
	}
	/// The regular intersection methods, as defined in the Shape parent class.
	bool intersect(Ray &ray, float *thit, Surfel *isect)
	{
    // if (ob.intersect_p(ray))
    // {
		Vec edge1, edge2, tvec, pvec, qvec;
		float det, inv_det, t, u, v_;

		edge1 = mesh->vertices[v[1]] - mesh->vertices[v[0]];
		edge2 = mesh->vertices[v[2]] - mesh->vertices[v[0]];

		pvec = cross(ray.getDirection(), edge2);

		det = dot(edge1, pvec);

		float epsilon = 0.0001;

		if (backface_cull)
		{
			if (det < epsilon)
				return 0;

			tvec = ray.getDirection() - mesh->vertices[v[0]];

			u = dot(tvec, pvec);

			if (u < 0.0 || u > det)
			{
				return 0;
			}

			qvec = cross(tvec, edge1);

			v_ = dot(ray.getDirection(), qvec);

			if (v_ < 0.0 || u + v_ > det)
			{
				return 0;
			}

			t = dot(edge2, qvec);
			inv_det = 1.0 / det;

			t *= inv_det;
			u *= inv_det;
			v_ *= inv_det;
		}
		else
		{
			if (det > -epsilon && det < epsilon)
				return 0;

			inv_det = 1.0 / det;

			tvec = ray.getOrigin() - mesh->vertices[v[0]];

			u = dot(tvec, pvec) * inv_det;

			if (u < 0.0 || u > 1.0)
			{
				return false;
			}

			qvec = cross(tvec, edge1);

			v_ = dot(ray.getDirection(), qvec) * inv_det;
			if (v_ <= 0.0 || u + v_ >= 1.0)
				return false;

			t = dot(edge2, qvec) * inv_det;
			
			// std::cout << "Calcula\n";
		}

		if(t < epsilon) return false;

		*thit = t;
		
		isect->p = ray(t);
		// isect->wo = unique_vector((ray.getDirection() - ray.getOrigin())* -1.0);
		isect->n = unique_vector(
			(mesh->normals[v[0]] * (1 - u - v_) +
			 mesh->normals[v[1]] * u +
			 mesh->normals[v[2]] * v_));// / (t + u + v_));
		// if(t > epsilon) return true;
		return true;
		/*} else {
		return false;
		}*/
	}

	bool intersect_p(Ray &ray)
	{
		Vec edge1, edge2, tvec, pvec, qvec;
		float det, inv_det, t, u, v_;

		edge1 = mesh->vertices[v[1]] - mesh->vertices[v[0]];
		edge2 = mesh->vertices[v[2]] - mesh->vertices[v[0]];

		pvec = cross(ray.getDirection(), edge2);

		det = dot(edge1, pvec);

		float epsilon = 0.000001;

		if (backface_cull)
		{
			if (det < epsilon)
				return 0;

			tvec = ray.getDirection() - mesh->vertices[v[0]];

			u = dot(tvec, pvec);

			if (u < 0.0 || u > det)
			{
				return 0;
			}

			qvec = cross(tvec, edge1);

			v_ = dot(ray.getDirection(), qvec);

			if (v_ < 0.0 || u + v_ > det)
			{
				return 0;
			}

			t = dot(edge2, qvec);
			inv_det = 1.0 / det;

			t *= inv_det;
			u *= inv_det;
			v_ *= inv_det;
		}
		else
		{
			if (det > -epsilon && det < epsilon)
				return false;

			inv_det = 1.0 / det;

			tvec = ray.getOrigin() - mesh->vertices[v[0]];

			u = dot(tvec, pvec) * inv_det;

			if (u < 0.0 || u > 1.0)
			{
				return false;
			}

			qvec = cross(tvec, edge1);

			v_ = dot(ray.getDirection(), qvec) * inv_det;
			if (v_ < 0.0 || u + v_ > 1.0)
				return false;

			t = dot(edge2, qvec) * inv_det;

			if(t > epsilon) return true;
		}

		return false;
	}

	void printCenter()
	{

		std::cout << "(" << mesh->vertices[v[0]].v1 << "," << mesh->vertices[v[0]].v2 << "," << mesh->vertices[v[0]].v3 << ")"
				  << "(" << mesh->vertices[v[1]].v1 << "," << mesh->vertices[v[1]].v2 << "," << mesh->vertices[v[1]].v3 << ")"
				  << "(" << mesh->vertices[v[2]].v1 << "," << mesh->vertices[v[2]].v2 << "," << mesh->vertices[v[2]].v3 << ")" << std::endl;
	}
	/// This friend function helps us debug the triangles, if we want to.
	friend std::ostream &operator<<(std::ostream &os, const Triangle &t);

	std::vector<std::shared_ptr<Shape>> create_triangle_mesh_shape(bool flip_normals, Paramset<std::string, std::string> ps);
	std::vector<std::shared_ptr<Shape>> create_triangle_mesh(std::shared_ptr<TriangleMesh> mesh, bool backface_cull);
	bool load_mesh_data(const std::string &filename, bool rvo, bool cn, bool fn, std::shared_ptr<TriangleMesh> md) { return true; }
	void extract_obj_data(const tinyobj::attrib_t &attrib,
						  const std::vector<tinyobj::shape_t> &shapes,
						  bool rvo, bool cn, bool fn, std::shared_ptr<TriangleMesh> md);
};

std::ostream &operator<<(std::ostream &os, const Triangle &t)
{
	constexpr short X{0};
	constexpr short Y{1};
	constexpr short Z{2};

	os << "    + V0: ("
	   << t.mesh->vertices[t.v[0]].v1 << ", "
	   << t.mesh->vertices[t.v[0]].v2 << ", "
	   << t.mesh->vertices[t.v[0]].v3 << ")\n"
	   << "    + V1: ("
	   << t.mesh->vertices[t.v[1]].v1 << ", "
	   << t.mesh->vertices[t.v[1]].v2 << ", "
	   << t.mesh->vertices[t.v[1]].v3 << ")\n"
	   << "    + V2: ("
	   << t.mesh->vertices[t.v[2]].v1 << ", "
	   << t.mesh->vertices[t.v[2]].v2 << ", "
	   << t.mesh->vertices[t.v[2]].v3 << ")\n"

	   << "    + N0: ("
	   << t.mesh->normals[t.n[0]].v1 << ", "
	   << t.mesh->normals[t.n[0]].v2 << ", "
	   << t.mesh->normals[t.n[0]].v3 << ")\n"
	   << "    + N1: ("
	   << t.mesh->normals[t.n[1]].v1 << ", "
	   << t.mesh->normals[t.n[1]].v2 << ", "
	   << t.mesh->normals[t.n[1]].v3 << ")\n"
	   << "    + N2: ("
	   << t.mesh->normals[t.n[2]].v1 << ", "
	   << t.mesh->normals[t.n[2]].v2 << ", "
	   << t.mesh->normals[t.n[2]].v3 << ")\n"

	   << "    + UV0: ("
	   << t.mesh->uvcoords[t.uv[0]].u << ", "
	   << t.mesh->uvcoords[t.uv[0]].v << ")\n"
	   << "    + UV1: ("
	   << t.mesh->uvcoords[t.uv[1]].u << ", "
	   << t.mesh->uvcoords[t.uv[1]].v << ")\n"
	   << "    + UV2: ("
	   << t.mesh->uvcoords[t.uv[2]].u << ", "
	   << t.mesh->uvcoords[t.uv[2]].v << ")\n";

	return os;
}

/// This is the function that actually creates the mesh database and the triangles, ans store them in a Shape list.
std::vector<std::shared_ptr<Shape>> create_triangle_mesh(std::shared_ptr<TriangleMesh> mesh, bool backface_cull)
{
	// List of shapes (triangles) we need to return to the client.
	std::vector<std::shared_ptr<Shape>> tris;
	// Create the triangles, which are just references to the mesh database.
	for (int i = 0; i < mesh->n_triangles; ++i)
		tris.push_back(std::make_shared<Triangle>(mesh, i, backface_cull));

	return tris;
}

/// This function essentially convertes the mesh data from the tinyobjloader internal data structure to our mesh data structure.
void extract_obj_data(const tinyobj::attrib_t &attrib,
					  const std::vector<tinyobj::shape_t> &shapes,
					  bool rvo, bool cn, bool fn, std::shared_ptr<TriangleMesh> md) 
{

	std::cout << "-- SUMMARY of the OBJ file --\n";
    std::cout << "# of vertices  : " << (attrib.vertices.size()  / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size()   / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2) << std::endl;
    std::cout << "# of shapes    : " << shapes.size()                 << std::endl;
    std::cout << "-----------------------------\n";

    // Retrieve the complete list of vertices.
    auto n_vertices{ attrib.vertices.size()/3 };
    for ( auto idx_v{0u} ; idx_v < n_vertices; idx_v++)
    {
        std::cout << "   v[" << static_cast<long>(idx_v) << "] = ( "
             << static_cast<double>(attrib.vertices[3 * idx_v + 0]) << ", "
             << static_cast<double>(attrib.vertices[3 * idx_v + 1]) << ", "
             << static_cast<double>(attrib.vertices[3 * idx_v + 2]) << " )\n";

        // Store the vertex in the mesh data structure.
        md->vertices.push_back( Vec{ attrib.vertices[ 3 * idx_v + 0 ],
                                        attrib.vertices[ 3 * idx_v + 1 ],
                                        attrib.vertices[ 3 * idx_v + 2 ] } );
    }


    // Read the normals
    // TODO: flip normals if requested.
    float flip = ( fn ) ? -1 : 1 ;
    auto n_normals{ attrib.normals.size()/3 };

    // Do we need to compute the normals? Yes only if the user requeste or there are no normals in the file.
    // if ( cn == true or n_normals == 0)
    // {
    //    // TODO: COmpute normals here.
    //    // compute_normals();
    // }
    //else {// Read normals from file. This corresponds to the entire 'for' below.

    // Traverse the normals read from the OBJ file.
    for ( auto idx_n{0u} ; idx_n < n_normals; idx_n++)
    {
        std::cout << "   n[" << static_cast<long>(idx_n) << "] = ( "
             << static_cast<double>(attrib.normals[3 * idx_n + 0]) << ", "
             << static_cast<double>(attrib.normals[3 * idx_n + 1]) << ", "
             << static_cast<double>(attrib.normals[3 * idx_n + 2]) << " )\n";

        // Store the normal.
        md->normals.push_back( Vec{ attrib.normals[ 3 * idx_n + 0 ] * flip,
                                        attrib.normals[ 3 * idx_n + 1 ] * flip,
                                        attrib.normals[ 3 * idx_n + 2 ] * flip } );
    }

    // Read the complete list of texture coordinates.
    auto n_texcoords{ attrib.texcoords.size()/2 };
    for ( auto idx_tc{0u} ; idx_tc < n_texcoords; idx_tc++)
    {
        std::cout << "   t[" << static_cast<long>(idx_tc) << "] = ( "
             << static_cast<double>(attrib.texcoords[2 * idx_tc + 0]) << ", "
             << static_cast<double>(attrib.texcoords[2 * idx_tc + 1]) << " )\n";

        // Store the texture coords.
        md->uvcoords.push_back( Point2{ attrib.texcoords[ 2 * idx_tc + 0 ],
                                         attrib.texcoords[ 2 * idx_tc + 1 ] } );
    }

    // Read mesh connectivity and store it as lists of indices to the real data.
    auto n_shapes{ shapes.size() };
    md->n_triangles = 0; // We must reset this here.
    // In case the OBJ file has the triangles organized in several shapes or groups, we
    // ignore this and store all triangles as a single mesh dataset.
    // This is why we need to reset the triangle count here.
    for ( auto idx_s{0u} ; idx_s < n_shapes; idx_s++)
    {
        std::cout << "The shape[" << idx_s << "].name = " << shapes[idx_s].name << '\n';
        std::cout << "Size of shape["<< idx_s << "].mesh.indices: "
             << static_cast<unsigned long>(shapes[idx_s].mesh.indices.size()) << '\n';
        std::cout << "shape["<< idx_s << "].num_faces: "
            <<  static_cast<unsigned long>(shapes[idx_s].mesh.num_face_vertices.size()) << '\n';

        // For each face print out the indices lists.
        size_t index_offset = 0;
        // # of triangles for this "shape" (group).
        // NOTE that we are accumulate the number of triangles coming from the shapes present in the OBJ file.
        md->n_triangles += shapes[idx_s].mesh.num_face_vertices.size();
        for ( auto idx_f{0} ; idx_f < md->n_triangles; idx_f++)
        {
            // Number of vertices per face (always 3, in our case)
            size_t fnum = shapes[idx_s].mesh.num_face_vertices[idx_f];

            std::cout << "  face[" << idx_f << "].fnum = "  << static_cast<unsigned long>(fnum) << '\n';

            // TODO: Invert order of vertices if flag is on. (DONE!)
            if ( rvo == true ) {
                std::cout << "Reverse order\n";
                // For each vertex in the face print the corresponding indices
                for (int v = fnum-1; v >= 0 ; v--)
                {
                    tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
                    std::cout << "    face[" << idx_f << "].v[" << v << "].indices = "
                        << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
                    // Add the indices to the global list of indices we need to pass on to the mesh object.
                    md->vertex_indices.push_back( idx.vertex_index );
                    md->normal_indices.push_back( idx.normal_index );
                    md->uvcoord_indices.push_back( idx.texcoord_index );
                }
            }
            else { // Keep the original vertex order
                // For each vertex in the face get the corresponding indices
                for (size_t v = 0; v < fnum; v++)
                {
                    tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
                    std::cout << "    face[" << idx_f << "].v[" << v << "].indices = "
                        << idx.vertex_index << "/" << idx.normal_index << "/" << idx.texcoord_index << '\n';
                    // Add the indices to the global list of indices we need to pass on to the mesh object.
                    // This goes to the mesh data structure.
                    md->vertex_indices.push_back( idx.vertex_index );
                    md->normal_indices.push_back( idx.normal_index );
                    md->uvcoord_indices.push_back( idx.texcoord_index );
                }
            }

            // Advance over to the next triangle.
            index_offset += fnum;
        }
    }

    std::cout << "This is the list of indices: \n";

    std::cout << "   + Vertices [ ";
    std::copy( md->vertex_indices.begin(), md->vertex_indices.end(), std::ostream_iterator< int > ( std::cout, " " ) );
    std::cout << "]\n";

    std::cout << "   + Normals [ ";
    std::copy( md->normal_indices.begin(), md->normal_indices.end(), std::ostream_iterator< int > ( std::cout, " " ) );
    std::cout << "]\n";

    std::cout << "   + UV coords [ ";
    std::copy( md->uvcoord_indices.begin(), md->uvcoord_indices.end(), std::ostream_iterator< int > ( std::cout, " " ) );
    std::cout << "]\n";				

}


/// Internal function that calls the tinyobjloader api to read the OBJ data into memory.
bool load_mesh_data(const std::string &filename, bool rvo, bool cn, bool fn, std::shared_ptr<TriangleMesh> md) {

	// Default load parameters
    const char* basepath = NULL;
    bool triangulate = true;

    std::cout << "Loading " << filename << std::endl;

    // This is the tinyobj internal data structures.
    tinyobj::attrib_t attrib; // Hold all the data, such as vertex, normals, UV coords
    std::vector<tinyobj::shape_t> shapes; // Represents a collection of faces, grouped in a unit.
    std::vector<tinyobj::material_t> materials; // Materials, NOT USED HERE.

    // Warnings and Error values.
    std::string warn;
    std::string err;

    // Timing loading.
    //================================================================================
    auto start = std::chrono::steady_clock::now();
    //================================================================================
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(),
            basepath, triangulate);
    //================================================================================
    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
    //Store the time difference between start and end
    std::chrono::duration<double> diff = end - start;

    // Milliseconds (10^-3)
    std::cout << "\t\t>>> " << std::chrono::duration <double, std::milli> (diff).count()
        << " ms" << std::endl;

    // Nanoseconds (10^-9)
    std::cout << "\t\t>>> " << std::chrono::duration <double, std::nano> (diff).count()
        << " ns" << std::endl;

    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    std::cout << "\t\t>>> " << diff_sec.count() << " s" << std::endl;
    //================================================================================

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
       std::cerr << "ERR: " << err << std::endl;
    }

    if (!ret) {
        std::cout << "Failed to load/parse .obj.\n";
        return false;
    }

    // Let us now "convert" or "migrate" the data from tinyobjloader data structure into out mesh data.
    extract_obj_data( attrib, shapes, // TinyObjeLoader data structures (IN)
                      rvo, cn, fn,    // Mesh modifiers (IN)
                      md );           // Reference to the mesh data to fill in. (OUT)

    return true;

}

const std::vector<std::string> explode(const std::string &s, const char &c)
{
	std::string buff{""};
	std::vector<std::string> v;

	for (auto n : s)
	{
		if (n != c)
			buff += n;
		else if (n == c && buff != "")
		{
			v.push_back(buff);
			buff = "";
		}
	}
	if (buff != "")
		v.push_back(buff);

	return v;
}

/// This is the entry point for the client. This function begins the process of reading a triangle mesh.
std::vector<std::shared_ptr<Shape>> create_triangle_mesh_shape(bool flip_normals, Paramset<std::string, std::string> ps)
{
	bool bkfc_on{false};
	bool reverse_vertex_order{false};
	bool compute_normals{false};

	std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>();
	std::string filename = ps.find("filename");
	std::string bkf_on_str = ps.find("backface_cull");

	if (bkf_on_str == "false" || bkf_on_str == "off")
	{
		bkfc_on = false;
	}

	std::string rvo_str = ps.find("reverse_vertex_order");
	if (rvo_str == "on" or rvo_str == "true")
		reverse_vertex_order = true;

	// Retrieve compute normals flag
	std::string cn_str = ps.find("compute_normals");
	if (cn_str == "on" or cn_str == "true")
		compute_normals = true;

	if (filename != "")
	{ // Read mesh data from an OBJ file provided in the scene file.
		// Call our auxiliary function that interfaces with tinyobjloader API.
		if (load_mesh_data(filename, reverse_vertex_order, compute_normals, flip_normals, mesh))
			std::cout << ">>> Mesh data successfuly loaded!\n";
		else
			std::cout << ">>> Mesh data loading failed!\n";
	}
	else
	{ // This means we must read the mesh data from the scene file, not from the OBJ file.
		int num_triangles = std::stoi(ps.find("ntriangles"));
		std::string indices = ps.find("indices");
		//std::vector<std::string> i{explode(indices, '  ')};
		std::string vertices = ps.find("vertices");
		std::string normals = ps.find("normals");
		std::string uv = ps.find("uv");

		std::vector<std::string> i_{explode(indices, ' ')};
		std::vector<std::string> v_{explode(vertices, ' ')};
		std::vector<std::string> n_{explode(normals, ' ')};
		std::vector<std::string> uv_{explode(uv, ' ')};

		std::vector<Vec> ind;
		for (int i = 0; i < i_.size();)
		{
			Vec v(std::stof(i_[i + 0]), std::stof(i_[i + 1]), std::stof(i_[i + 2]));
			i += 3;
			ind.push_back(v);
		}

		std::vector<Vec> vert;
		for (int i = 0; i < v_.size();)
		{
			Vec v(std::stof(v_[i + 0]), std::stof(v_[i + 1]), std::stof(v_[i + 2]));
			i += 3;
			vert.push_back(v);
		}

		for (int i = 0; i < vert.size(); i++)
		{
			vert[i].print();
		}

		std::vector<Vec> norm;
		for (int i = 0; i < n_.size();)
		{
			Vec v(std::stof(n_[i + 0]), std::stof(n_[i + 1]), std::stof(n_[i + 2]));
			i += 3;
			norm.push_back(v);
		}

		std::vector<Point2> uvs;
		for (int i = 0; i < uv_.size();)
		{
			Point2 v(std::stof(uv_[i + 0]), std::stof(uv_[i + 1]));
			i += 2;
			uvs.push_back(v);
		}

		mesh->n_triangles = num_triangles;
		mesh->vertices = vert;
		mesh->normals = norm;
		mesh->uvcoords = uvs;

		for (int i = 0; i < ind.size(); i++)
		{
			mesh->vertex_indices.push_back(ind[i].v1);
			mesh->vertex_indices.push_back(ind[i].v2);
			mesh->vertex_indices.push_back(ind[i].v3);

			mesh->normal_indices.push_back(ind[i].v1);
			mesh->normal_indices.push_back(ind[i].v2);
			mesh->normal_indices.push_back(ind[i].v3);

			mesh->uvcoord_indices.push_back(ind[i].v1);
			mesh->uvcoord_indices.push_back(ind[i].v2);
			mesh->uvcoord_indices.push_back(ind[i].v3);
		}
	}

	return create_triangle_mesh(mesh, bkfc_on);
}

#endif
