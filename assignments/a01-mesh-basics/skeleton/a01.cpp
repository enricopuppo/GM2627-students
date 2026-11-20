#include <gm/a01.hpp>
#include <gm/validation.hpp>
namespace gm {
Adjacency vertex_faces(const Mesh& mesh) {
  validate_mesh(mesh);
  // TODO: one list per vertex, containing its incident face indices.
  throw NotImplemented("vertex_faces: not implemented");
}
Adjacency vertex_neighbors(const Mesh& mesh) {
  validate_mesh(mesh);
  // TODO: unique edge neighbors, no self entries.
  throw NotImplemented("vertex_neighbors: not implemented");
}
NormalField shading_normals(const Mesh& mesh, NormalMode mode, double crease_degrees) {
  validate_mesh(mesh); (void)mode; (void)crease_degrees;
  // TODO: return the requested association and corresponding unit normals.
  throw NotImplemented("shading_normals: not implemented");
}
Mesh split_faces(const Mesh& mesh) {
  validate_closed(mesh);
  // TODO: append barycenters and construct three consistently wound triangles per face.
  throw NotImplemented("split_faces: not implemented");
}
Eigen::MatrixXd relocate_vertices(const Mesh& mesh) {
  validate_closed(mesh);
  // TODO: use original vertex positions and original neighborhoods.
  throw NotImplemented("relocate_vertices: not implemented");
}
Eigen::MatrixXi flip_old_edges(const Mesh& mesh) {
  validate_closed(mesh);
  // TODO: build final triangles by replacing each old edge with a barycenter edge.
  throw NotImplemented("flip_old_edges: not implemented");
}
// Supplied orchestration; students implement the three stages above.
Mesh subdivide_sqrt3(const Mesh& mesh) {
  Mesh out=split_faces(mesh);
  out.V.topRows(mesh.V.rows())=relocate_vertices(mesh);
  out.F=flip_old_edges(mesh);
  validate_closed(out); return out;
}
}
