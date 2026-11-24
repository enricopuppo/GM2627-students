#pragma once
#include <Eigen/Core>
#include <stdexcept>
namespace gm {
struct Mesh { Eigen::MatrixXd V; Eigen::MatrixXi F; };
struct NotImplemented : std::logic_error { using std::logic_error::logic_error; };
}
