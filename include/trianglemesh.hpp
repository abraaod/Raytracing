#ifndef _TRIANGLE_M_
#define _TRIANGLE_M_

#include <vector>
#include "vec.hpp"

using Point3f = Vec;
using Normal3f = Vec;
using Point2f = Vec;

struct TriangleMesh {
    int n_triangles; //!< # of triangles in the mesh.
    // The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
    std::vector< int > vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
    std::vector< int > normal_indices;  //!< The list of indices to the normals list, for each individual triangle.
    std::vector< int > uvcoord_indices; //!< The list of indices to the UV coord list, for each individual triangle.

    // Here comes the data associated with each vertex. WARNING: The length of the vectors below may differ.
    std::vector< Point3f > vertices;  //!< The 3D geometric coordinates
    std::vector< Normal3f > normals;  //!< The 3D normals.
    std::vector< Point2f > uvcoords; //!< The 2D texture coordinates.

    // Regular constructor (almost all data is imported via move semantics).
    TriangleMesh(){/*empty*/};
    /// Copy constructor disabled
    TriangleMesh( const TriangleMesh& ) = delete;
    /// Assign operator disabled.
    TriangleMesh& operator=( const TriangleMesh & ) = delete;
    /// Move constructor.
    TriangleMesh( TriangleMesh && other ) = delete;
};

#endif