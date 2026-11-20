#pragma once
#include <Eigen/Core>
#include <stdexcept>
#include <vector>
namespace gm {
struct Mesh { Eigen::MatrixXd V; Eigen::MatrixXi F; };
using Adjacency = std::vector<std::vector<int>>;
struct NotImplemented : std::logic_error { using std::logic_error::logic_error; };
enum class NormalMode { Face, Vertex, Corner };
// rows = #F, #V, or 3*#F, respectively. Corner row = 3*f+c.
struct NormalField { NormalMode association; Eigen::MatrixXd values; };
// All inputs must satisfy validate_mesh(). Isolated vertices have empty lists.
Adjacency vertex_faces(const Mesh& mesh);
Adjacency vertex_neighbors(const Mesh& mesh);
// Area-weighted vertex/corner normals. crease_degrees in [0,180].
// Zero isolated/cancelling vertex normals; corner cancellation falls back to face.
NormalField shading_normals(const Mesh& mesh, NormalMode mode, double crease_degrees = 45);
// Subdivision stages require a closed oriented vertex-manifold, no isolated vertices.
// split: old positions first, then face barycenters; face 3*f+c = [F(f,c),F(f,(c+1)%3),#V+f].
Mesh split_faces(const Mesh& mesh);
// Returns #V relocated old positions, computed entirely from the original mesh.
Eigen::MatrixXd relocate_vertices(const Mesh& mesh);
// Returns final connectivity using old indices and #V+face barycenter indices.
Eigen::MatrixXi flip_old_edges(const Mesh& mesh);
// One step; append barycenters in original face order and preserve winding.
Mesh subdivide_sqrt3(const Mesh& mesh);
}
