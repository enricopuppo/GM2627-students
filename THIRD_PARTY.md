# Third-party dependencies and data

Dependencies are downloaded at configure time, not bundled into student source archives.
Keep their upstream copyright/license files if distributing compiled binaries or vendored sources.

| Dependency | Pin | License / upstream notice |
|---|---|---|
| libigl | v2.5.0, fdaac01bcc52888994f7afd029dcc045dd408484 | MPL-2.0, https://github.com/libigl/libigl/blob/v2.5.0/LICENSE.MPL2 |
| Eigen | 3.4.0, 3147391d946bb4b6c68edd901f2add6ac1f31f8c | MPL-2.0, https://gitlab.com/libeigen/eigen/-/blob/3.4.0/COPYING.MPL2 |
| GLFW (viewer only) | 3.3.10, dc46d3f8129712e42856c20e99a604a3b08ad581 | zlib, https://github.com/glfw/glfw/blob/dc46d3f8129712e42856c20e99a604a3b08ad581/LICENSE.md |
| libigl-glad (viewer only) | ead2d21fd1d9f566d8f9a9ce99ddf85829258c7a | generated loader notices in https://github.com/libigl/libigl-glad |
| Dear ImGui (viewer only) | v1.85, 55d35d8387c15bf0cfd71861df67af8cfbda7456 | MIT, https://github.com/ocornut/imgui/blob/v1.85/LICENSE.txt |
| ImGuizmo (viewer only) | a23567269f6617342bcc112394bdad937b54b2d7 | MIT, https://github.com/CedricGuillemet/ImGuizmo/blob/a23567269f6617342bcc112394bdad937b54b2d7/LICENSE |

Only permissive libigl modules core/opengl/glfw/imgui are enabled. No CGAL, MATLAB,
TetGen, CoMISo, or external numerical solver is used. CMake 4 compatibility is
set to 3.5 for old third-party CMakeLists; it does not change the C++17 requirement.
Pinned viewer recipes are part of the libigl revision above. Eigen, ImGui and GLFW
release tags are overridden with immutable commit IDs in our dependency file.
The bundled libigl ImGui font data is pinned at
7e1053e750b0f4c129b046f4e455243cb7f804f3 in https://github.com/libigl/libigl-imgui;
retain that repository's font license notices if bundling it.

Canonical input files are unchanged. data/manifest.json records their SHA-256
and source paths. Their redistribution provenance has not been cleared; generated
packages are for instructor review until it is cleared. This file is an inventory,
not a replacement for upstream license texts and not a new course code-use policy.
