#include <array>
#include <fstream>
#include <functional>
#include <gm/a01.hpp>
#include <gm/io.hpp>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <imgui.h>
#include <iostream>
#include <string>

// Teacher owns the renderer and the association-to-corner mapping.
static Eigen::MatrixXd display_normals(const gm::Mesh &m, const gm::NormalField &n) {
    int rows = n.association == gm::NormalMode::Face     ? int(m.F.rows())
               : n.association == gm::NormalMode::Vertex ? int(m.V.rows())
                                                         : 3 * int(m.F.rows());
    if (n.values.rows() != rows || n.values.cols() != 3 || !n.values.allFinite()) {
        throw std::runtime_error("Invalid normal field dimensions/values");
    }
    Eigen::MatrixXd corners(3 * m.F.rows(), 3);
    for (int f = 0; f < m.F.rows(); ++f) {
        for (int c = 0; c < 3; ++c) {
            int r = n.association == gm::NormalMode::Face     ? f
                    : n.association == gm::NormalMode::Vertex ? m.F(f, c)
                                                              : 3 * f + c;
            corners.row(3 * f + c) = n.values.row(r);
        }
    }
    return corners;
}

// Run the teacher-owned viewer with either student or reference computations.
int main(int argc, char **argv) {
    try {
        // 1. Application data and editable GUI settings.
        igl::opengl::glfw::Viewer viewer;
        igl::opengl::glfw::imgui::ImGuiPlugin plugin;
        igl::opengl::glfw::imgui::ImGuiMenu menu;
        std::string path = argc > 1 ? argv[1] : std::string(GM_DATA_DIR) + "/cube.obj";
        gm::Mesh original = gm::load_mesh(path), current = original;
        std::string status = "Input loaded with teacher preview normals. Select a shading mode or "
                             "click Compute shading.";
        std::array<char, 1024> input{}, output{};
        std::copy_n(path.c_str(), std::min(path.size(), input.size() - 1), input.data());
        std::string out = "a01-result.off";
        std::copy(out.begin(), out.end(), output.begin());
        int mode = 0, dataset = 0, iterations = 1, stage = 0;
        float crease = 45;
        bool wire = true;
        bool graphics_ready = false;

        // 2. Attach the GUI plugin; initialize the OpenGL context before the event loop.
        viewer.plugins.push_back(&plugin);
        plugin.widgets.push_back(&menu);

        // 3. Helper callbacks used by the GUI.

        // Compute teacher preview normals so the untouched starter can display input.
        auto preview = [&](const gm::Mesh &m) {
            gm::NormalField n{gm::NormalMode::Face, Eigen::MatrixXd(m.F.rows(), 3)};
            for (int f = 0; f < m.F.rows(); ++f) {
                Eigen::Vector3d a = m.V.row(m.F(f, 1)) - m.V.row(m.F(f, 0)),
                                b = m.V.row(m.F(f, 2)) - m.V.row(m.F(f, 0));
                n.values.row(f) = a.cross(b).normalized();
            }
            return n;
        };
        // Adapt normals and mesh corners for display, optionally fitting the camera.
        auto render = [&](const gm::Mesh &m, const gm::NormalField &n, bool fit) {
            gm::validate_mesh(m);
            auto cn = display_normals(m, n);
            // Explicit display soup avoids libigl's row-count ambiguity when #V == 3*#F.
            Eigen::MatrixXd displayV(3 * m.F.rows(), 3);
            Eigen::MatrixXi displayF(m.F.rows(), 3);
            for (int f = 0; f < m.F.rows(); ++f) {
                for (int c = 0; c < 3; ++c) {
                    displayV.row(3 * f + c) = m.V.row(m.F(f, c));
                    displayF(f, c) = 3 * f + c;
                }
            }
            viewer.data().clear();
            viewer.data().set_mesh(displayV, displayF);
            viewer.data().set_face_based(false);
            viewer.data().set_normals(cn);
            viewer.data().show_lines = wire;
            if (fit) { // Recenter the camera only when requested.
                viewer.core().align_camera_center(m.V, m.F);
            }
        };
        // Wake the event-driven renderer after a GUI change.
        auto request_redraw = [&]() {
            // Menu callbacks run after the scene draw. Wake libigl's event-driven loop
            // so changed mesh data is drawn without requiring another user event.
            if (graphics_ready) {
                glfwPostEmptyEvent();
            }
        };
        // Execute a GUI action, report failures and request a fresh frame.
        auto run = [&](const std::function<void()> &action) {
            try {
                action();
            } catch (const std::exception &e) {
                status = e.what();
                std::cerr << status << '\n';
            }
            request_redraw();
        };
        // Compute student normals for the current mesh and selected shading settings.
        auto compute_shading = [&]() {
            auto n = gm::shading_normals(current, static_cast<gm::NormalMode>(mode), crease);
            render(current, n, false);
            status = "Student shading computed.";
        };
        // Load a new input and replace the reset snapshot after successful display setup.
        auto load = [&](const std::string &filename) {
            auto next = gm::load_mesh(filename);
            render(next, preview(next), true);
            original = next;
            current = next;
            path = filename;
            status = "Loaded " + path + " (teacher preview)";
        };
        render(current, preview(current), true);
        // 4. GUI construction.
        // Draw input, shading, refinement, reset and export controls.
        menu.callback_draw_viewer_menu = [&]() {
            ImGui::TextUnformatted("Assignment 1 - teacher controls");
            const char *names[] = {"cube.obj", "sphere.obj", "bunny-small.off"};
            ImGui::Combo("Dataset", &dataset, names, 3);
            // Load the selected built-in model.
            if (ImGui::Button("Load dataset")) {
                run([&] {
                    load(std::string(GM_DATA_DIR) + "/" + names[dataset]);
                });
            }
            ImGui::InputText("Input path", input.data(), input.size());
            // Load the mesh named in the path field.
            if (ImGui::Button("Load path")) {
                run([&] {
                    load(input.data());
                });
            }
            ImGui::Text("%d vertices, %d faces", int(current.V.rows()), int(current.F.rows()));
            if (ImGui::Checkbox("Wireframe", &wire)) {
                viewer.data().show_lines = wire;
                request_redraw();
            }
            const bool mode_changed = ImGui::Combo("Normals", &mode, "Flat\0Vertex\0Corner\0");
            const bool crease_changed = ImGui::SliderFloat("Crease (degrees)", &crease, 0, 180);
            const bool compute_clicked = ImGui::Button("Compute shading");
            // Recompute shading when its controls change or the user requests a retry.
            if (mode_changed || crease_changed || compute_clicked) {
                run(compute_shading);
            }
            // Print computed incident-face and edge-neighbor lists to the launch terminal.
            if (ImGui::Button("Print adjacency")) {
                run([&] {
                    auto vf = gm::vertex_faces(current), vv = gm::vertex_neighbors(current);
                    if (vf.size() != size_t(current.V.rows()) || vv.size() != vf.size()) {
                        throw std::runtime_error("Invalid adjacency dimensions");
                    }
                    for (size_t i = 0; i < vf.size(); ++i) {
                        std::cout << i << " faces:";
                        for (int f : vf[i]) {
                            std::cout << ' ' << f;
                        }
                        std::cout << " neighbors:";
                        for (int v : vv[i]) {
                            std::cout << ' ' << v;
                        }
                        std::cout << '\n';
                    }
                    status = "Adjacency written to launch terminal.";
                });
            }
            ImGui::Combo("Stage", &stage, "Full sqrt(3)\0Split only\0Split + relocate\0");
            ImGui::SliderInt("Iterations", &iterations, 1, 4);
            // Compute the selected refinement stage before replacing the current mesh.
            if (ImGui::Button("Refine current")) {
                run([&] {
                    auto next = current;
                    // Intermediate stages are diagnostic previews of a single step only.
                    int count = stage == 0 ? iterations : 1;
                    for (int i = 0; i < count; ++i) {
                        if (next.F.rows() > 300000) {
                            throw std::runtime_error("Viewer refinement safety limit reached; use "
                                                     "batch for larger meshes");
                        }
                        if (stage == 0) {
                            next = gm::subdivide_sqrt3(next);
                        } else {
                            auto split = gm::split_faces(next);
                            if (stage == 2) {
                                split.V.topRows(next.V.rows()) = gm::relocate_vertices(next);
                            }
                            next = split;
                        }
                    }
                    render(next, preview(next), false);
                    current = next;
                    status = "Refinement computed; teacher preview normals. Compute shading to "
                             "inspect normals.";
                });
            }
            // Restore the loaded input with teacher preview shading.
            if (ImGui::Button("Reset")) {
                run([&] {
                    render(original, preview(original), true);
                    current = original;
                    status = "Reset to input (teacher preview).";
                });
            }
            ImGui::InputText("Output OFF", output.data(), output.size());
            // Save the current computational mesh as OFF.
            if (ImGui::Button("Export mesh")) {
                run([&] {
                    gm::save_mesh(output.data(), current);
                    status = "Exported " + std::string(output.data());
                });
            }
            ImGui::Separator();
            ImGui::TextWrapped("%s", status.c_str());
        };
        // 5. Remaining command-line parameters and the optional rendering check.
        // The --smoke flag runs a quick startup/rendering check without an event-loop wait.
        // Usage: viewer input --smoke output.ppm [face|vertex|corner] [full_iterations]
        if (argc > 2 && std::string(argv[2]) == "--smoke") { // Run the automatic rendering check.
            if (argc < 4) { // Require a path for the saved image.
                throw std::invalid_argument("--smoke requires an output.ppm path");
            }
            if (argc > 5) { // Optionally refine before checking rendering.
                int steps = std::stoi(argv[5]);
                if (steps < 0 || steps > 3) { // Keep the rendering check bounded.
                    throw std::invalid_argument("Rendering-check iterations must be 0..3");
                }
                for (int i = 0; i < steps; ++i) {
                    current = gm::subdivide_sqrt3(current);
                }
            }
            if (argc > 4) { // Select student normals instead of the teacher preview.
                std::string choice = argv[4];
                auto association = choice == "face"     ? gm::NormalMode::Face
                                   : choice == "vertex" ? gm::NormalMode::Vertex
                                                        : gm::NormalMode::Corner;
                if (choice != "face" && choice != "vertex" && choice != "corner") { // Reject an unknown normal mode.
                    throw std::invalid_argument("Unknown rendering-check normal mode");
                }
                render(current, gm::shading_normals(current, association, crease), true);
            } else {
                // An untouched starter must report TODO without changing its loaded mesh.
                try {
                    gm::shading_normals(current, gm::NormalMode::Face);
                } catch (const gm::NotImplemented &e) {
                    std::cout << e.what() << "; preview retained\n";
                }
            }
            if (viewer.launch_init(false, "Assignment 1 rendering check", 1000, 800) != 0) { // Abort if OpenGL initialization fails.
                return 1;
            }
            graphics_ready = true;
            viewer.core().is_animating = true;
            viewer.core().trackball_angle =
                Eigen::Quaternionf(Eigen::AngleAxisf(-.3f, Eigen::Vector3f::UnitX()) *
                                   Eigen::AngleAxisf(.5f, Eigen::Vector3f::UnitY()));
            for (int i = 0; i < 3; ++i) {
                viewer.launch_rendering(false);
            }
            Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> R(800, 600), G(800, 600),
                B(800, 600), A(800, 600);
            viewer.core().draw_buffer(viewer.data(), true, R, G, B, A);
            if (glGetError() != GL_NO_ERROR) { // Report a failed OpenGL rendering operation.
                throw std::runtime_error("OpenGL error during rendering check");
            }
            std::ofstream file(argv[3], std::ios::binary);
            file << "P6\n" << R.rows() << " " << R.cols() << "\n255\n";
            for (int y = int(R.cols()) - 1; y >= 0; --y) {
                for (int x = 0; x < R.rows(); ++x) {
                    const char rgb[] = {char(R(x, y)), char(G(x, y)), char(B(x, y))};
                    file.write(rgb, 3);
                }
            }
            if (!file) { // Report an image-write failure.
                throw std::runtime_error("Cannot write rendering-check image");
            }
            viewer.launch_shut();
            std::cout << "Native rendering check passed: " << argv[3] << '\n';
            return 0;
        }
        // 6. Initialize OpenGL, run the interactive event loop and clean up.
        if (viewer.launch_init(false, "Assignment 1") != 0) {
            return 1;
        }
        graphics_ready = true;
        viewer.launch_rendering(true);
        viewer.launch_shut();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
