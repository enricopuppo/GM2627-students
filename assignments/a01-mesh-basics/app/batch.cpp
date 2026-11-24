#include <gm/a01.hpp>
#include <gm/io.hpp>
#include <iostream>
#include <string>

// Run computational tasks without graphics; unfinished student operations return exit code 2.
int main(int argc, char **argv) {
    try {
        if (argc < 2) { // Require an input mesh path.
            std::cerr << "Usage: batch input [load|adjacency|normals|subdivide] [iterations] "
                         "[output.off]\n";
            return 1;
        }
        auto m = gm::load_mesh(argv[1]);
        std::string op = argc > 2 ? argv[2] : "load"; // Optional operation; default is load only.
        if (op == "adjacency") { // Print incident faces and distinct edge neighbors.
            auto vf = gm::vertex_faces(m), vv = gm::vertex_neighbors(m);
            for (int v = 0; v < m.V.rows(); ++v) {
                std::cout << v << " faces:";
                for (int f : vf.at(v)) {
                    std::cout << ' ' << f;
                }
                std::cout << " neighbors:";
                for (int w : vv.at(v)) {
                    std::cout << ' ' << w;
                }
                std::cout << '\n';
            }
        } else if (op == "normals") { // Exercise all three normal associations.
            for (auto mode :
                 {gm::NormalMode::Face, gm::NormalMode::Vertex, gm::NormalMode::Corner}) {
                auto n = gm::shading_normals(m, mode, 45);
                std::cout << "normal rows " << n.values.rows() << '\n';
            }
        } else if (op == "subdivide") { // Apply full sqrt(3) refinement steps.
            int n = argc > 3 ? std::stoi(argv[3]) : 1; // Optional number of iterations.
            if (n < 1 || n > 6) {
                throw std::invalid_argument("Iterations must be 1..6");
            }
            for (int i = 0; i < n; ++i) {
                if (m.F.rows() > 1000000) {
                    throw std::runtime_error("Refinement safety limit reached");
                }
                m = gm::subdivide_sqrt3(m);
            }
            if (argc > 4) { // Optional output OFF path.
                gm::save_mesh(argv[4], m);
            }
        } else if (op != "load") {
            throw std::invalid_argument("Unknown operation");
        }
        std::cout << "mesh " << m.V.rows() << " vertices " << m.F.rows() << " faces\n";
        return 0;
    } catch (const gm::NotImplemented &e) {
        std::cerr << e.what() << '\n';
        return 2;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
