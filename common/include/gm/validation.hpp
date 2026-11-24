#pragma once
#include <gm/mesh.hpp>
#include <Eigen/Geometry>
#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <map>
#include <set>
#include <queue>
namespace gm {
inline void validate_mesh(const Mesh& m) {
  if(m.V.cols()!=3 || m.F.cols()!=3 || m.V.rows()==0 || m.F.rows()==0 || !m.V.allFinite())
    throw std::invalid_argument("Expected nonempty finite V x 3 and F x 3 triangle mesh");
  if(m.V.rows()>std::numeric_limits<int>::max()/3 || m.F.rows()>std::numeric_limits<int>::max()/3)
    throw std::invalid_argument("Mesh exceeds supported integer index range");
  std::set<std::array<int,3>> faces;
  for(int f=0; f<m.F.rows(); ++f) {
    std::array<int,3> ids{{m.F(f,0),m.F(f,1),m.F(f,2)}};
    for(int v:ids) if(v<0 || v>=m.V.rows()) throw std::invalid_argument("Face index out of range");
    auto sorted=ids; std::sort(sorted.begin(),sorted.end());
    if(sorted[0]==sorted[1] || sorted[1]==sorted[2] || !faces.insert(sorted).second)
      throw std::invalid_argument("Repeated vertex or duplicate face");
    Eigen::Vector3d a=m.V.row(ids[1])-m.V.row(ids[0]), b=m.V.row(ids[2])-m.V.row(ids[0]);
    double s=std::max({a.norm(),b.norm(),(a-b).norm()});
    if(!std::isfinite(s) || s<=0 || !((a/s).cross(b/s).norm()>1e-14))
      throw std::invalid_argument("Degenerate or numerically unresolved triangle");
  }
}
struct Halfedge { int from,to,face; };
using EdgeMap=std::map<std::pair<int,int>,std::vector<Halfedge>>;
inline EdgeMap edge_map(const Mesh& m) {
  EdgeMap edges;
  for(int f=0;f<m.F.rows();++f) for(int c=0;c<3;++c) {
    int a=m.F(f,c),b=m.F(f,(c+1)%3);
    edges[std::minmax(a,b)].push_back({a,b,f});
  }
  return edges;
}
inline void validate_closed(const Mesh& m) {
  validate_mesh(m);
  const auto edges=edge_map(m);
  for(const auto& item:edges) {
    const auto& h=item.second;
    if(h.size()!=2 || h[0].from!=h[1].to || h[0].to!=h[1].from)
      throw std::invalid_argument("Subdivision requires closed consistently oriented manifold edges");
  }
  // A single cyclic link at every vertex excludes bow-ties and isolated vertices.
  std::vector<std::map<int,std::vector<int>>> links(m.V.rows());
  for(int f=0;f<m.F.rows();++f) for(int c=0;c<3;++c) {
    int v=m.F(f,c),a=m.F(f,(c+1)%3),b=m.F(f,(c+2)%3);
    links[v][a].push_back(b); links[v][b].push_back(a);
  }
  for(const auto& link:links) {
    if(link.empty()) throw std::invalid_argument("Subdivision does not support isolated vertices");
    for(const auto& node:link) if(node.second.size()!=2) throw std::invalid_argument("Nonmanifold vertex link");
    std::set<int> seen; std::queue<int> q; q.push(link.begin()->first);
    while(!q.empty()) { int v=q.front();q.pop(); if(!seen.insert(v).second) continue; for(int w:link.at(v)) q.push(w); }
    if(seen.size()!=link.size()) throw std::invalid_argument("Disconnected vertex link");
  }
}
}
