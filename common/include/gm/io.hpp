#pragma once
#include <gm/validation.hpp>
#include <igl/read_triangle_mesh.h>
#include <igl/writeOFF.h>
#include <string>
namespace gm {
inline Mesh load_mesh(const std::string& path) {
  Mesh m;
  if(!igl::read_triangle_mesh(path,m.V,m.F)) throw std::runtime_error("Cannot load mesh: "+path);
  validate_mesh(m); return m;
}
inline void save_mesh(const std::string& path,const Mesh& m) {
  validate_mesh(m);
  if(!igl::writeOFF(path,m.V,m.F)) throw std::runtime_error("Cannot write mesh: "+path);
}
}
