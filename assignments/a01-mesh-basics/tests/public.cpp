#include "check.hpp"
#include <gm/validation.hpp>
#include <set>
int main() {try {
  auto m=tetra();auto vf=gm::vertex_faces(m),vv=gm::vertex_neighbors(m);
  check(vf.size()==4 && vv.size()==4,"Adjacency dimensions");
  check(std::set<int>(vf[0].begin(),vf[0].end())==std::set<int>({0,1,2}),"Incident faces at tetra vertex 0");
  for(int v=0;v<4;++v) {check(vv[v].size()==3,"Tetra valence");check(std::set<int>(vv[v].begin(),vv[v].end()).size()==3,"Duplicate neighbors");for(int w:vv[v])check(w!=v && w>=0 && w<4,"Invalid neighbor");}
  for(auto mode:{gm::NormalMode::Face,gm::NormalMode::Vertex,gm::NormalMode::Corner}) {
    auto n=gm::shading_normals(m,mode,45);
    check(n.association==mode && n.values.rows()==(mode==gm::NormalMode::Corner?12:4) && n.values.cols()==3,"Normal association/dimensions");
    for(int i=0;i<n.values.rows();++i)check(std::abs(n.values.row(i).norm()-1)<1e-12,"Unit normal");
  }
  auto s=gm::subdivide_sqrt3(m);check(s.V.rows()==8 && s.F.rows()==12,"Subdivision counts");gm::validate_closed(s);
  // Regular tetrahedron: each old vertex becomes 7/27 of its original position.
  check((s.V.topRows(4)-(7./27)*m.V).norm()<1e-12,"Tetra relocation analytic value");
  auto open=m;open.F=m.F.topRows(3);rejects([&]{gm::subdivide_sqrt3(open);});
  std::cout<<"Public checks passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
