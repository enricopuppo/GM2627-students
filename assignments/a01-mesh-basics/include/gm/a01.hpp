#pragma once
#include <gm/mesh.hpp>
#include <stdexcept>
#include <vector>

namespace gm {

// STUDENT INTERFACE: data types, followed by computational functions.
// Implement the TODOs in skeleton/a01.cpp; no viewer changes are required.
// Mesh is supplied by gm/mesh.hpp: V is #V x 3, F is #F x 3 (zero-based, CCW).
// Preserve input ordering. Validation errors use std::invalid_argument.

using Adjacency = std::vector<std::vector<int>>;

enum class NormalMode { Face, Vertex, Corner };

// Numerical normals and their face, vertex or corner association.
struct NormalField {
    NormalMode association;
    Eigen::MatrixXd values;
};

// Build one incident-face list per input vertex. Include every face once in
// each of its three vertices' lists; isolated vertices have empty lists.
// List order is immaterial. Input must satisfy validate_mesh(). You may use
// igl::vertex_triangle_adjacency, preserving lists for isolated vertices.
Adjacency vertex_faces(const Mesh &mesh);

// Build one list of distinct edge-neighbor vertex indices per input vertex.
// Exclude self entries and duplicates; isolated vertices have empty lists.
// List order is immaterial. Input must satisfy validate_mesh(). You may use
// igl::adjacency_list, preserving lists for isolated vertices.
Adjacency vertex_neighbors(const Mesh &mesh);

// Compute geometric unit normals for the requested association: #F face rows,
// #V vertex rows, or 3*#F corner rows, where row 3*f+c belongs to face f, corner c.
// Face normals follow the winding of F. Vertex normals are normalized sums of
// incident face normals weighted by area; isolated/cancelling vertices get zero.
// For corners, use the same area weights but include only incident faces whose
// unit-normal dot product with the current face is strictly greater than
// cos(crease_degrees*pi/180). This is not a transitive grouping. At zero degrees
// return the current face normal; cancelling corner sums fall back to it too.
// Treat normalized row lengths below 1e-14 as cancellation. crease_degrees must
// be finite and in [0,180]. Input must satisfy validate_mesh(); open meshes and
// isolated vertices are allowed. libigl normal helpers are permitted; explicitly
// select AREA weighting for igl::per_vertex_normals. Return normals, not colors.
NormalField shading_normals(const Mesh &mesh, NormalMode mode, double crease_degrees = 45);

// Insert each face barycenter and split that face into three wound triangles.
// Keep original positions first; append barycenters in original face-row order.
// Barycenter f has index #V+f; output face 3*f+c is
// [F(f,c), F(f,(c+1)%3), #V+f]. Input must satisfy validate_closed(): a closed,
// oriented triangular vertex-manifold without isolated vertices. Implement the
// insertion yourself; libigl core topology/barycenter helpers are permitted.
Mesh split_faces(const Mesh &mesh);

// Return #V relocated OLD positions using only the original mesh and neighbors.
// For valence n, alpha=(4-2*cos(2*pi/n))/9 and
// P_v=(1-alpha)*V_v + (alpha/n)*sum(V_w), over distinct original neighbors w.
// Do not relocate barycenters or overwrite positions needed by other vertices.
// Input must satisfy validate_closed(); implement this rule yourself.
Eigen::MatrixXd relocate_vertices(const Mesh &mesh);

// Return final connectivity after replacing each original old-old edge with
// the edge joining its two incident face barycenters. Keep the new spokes.
// Use original vertex indices and barycenter indices #V+f; produce two triangles
// per old edge, preserving winding. Final face-row order is immaterial. Input
// must satisfy validate_closed(). Core topology helpers are permitted, but
// implement the flipping logic yourself; no ready-made subdivision routine.
Eigen::MatrixXi flip_old_edges(const Mesh &mesh);

// Supplied composition of the three student stages: perform one complete step.
// All stages read the original mesh; replace only the old positions with their
// relocated values, retain appended barycenters, then install final connectivity.
// The validated output can be used as the next input. Do not modify this function.
Mesh subdivide_sqrt3(const Mesh &mesh);

} // namespace gm
