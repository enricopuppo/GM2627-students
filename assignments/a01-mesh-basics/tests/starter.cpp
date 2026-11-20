#include "check.hpp"
int main() {try {
  auto m=tetra(); int count=0;
  auto todo=[&](auto fn){try{fn();}catch(const gm::NotImplemented&){++count;}};
  todo([&]{gm::vertex_faces(m);});todo([&]{gm::vertex_neighbors(m);});
  todo([&]{gm::shading_normals(m,gm::NormalMode::Face);});
  todo([&]{gm::split_faces(m);});todo([&]{gm::relocate_vertices(m);});todo([&]{gm::flip_old_edges(m);});todo([&]{gm::subdivide_sqrt3(m);});
  check(count==7,"Starter should report every TODO explicitly");return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
