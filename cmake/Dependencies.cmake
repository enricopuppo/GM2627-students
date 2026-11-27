include(FetchContent)
FetchContent_Declare(eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3147391d946bb4b6c68edd901f2add6ac1f31f8c
  SOURCE_SUBDIR Eigen)
FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG 55d35d8387c15bf0cfd71861df67af8cfbda7456)
# GLFW 3.3.10 fixes Apple Silicon display-name lookup in libigl's 2020 snapshot.
# Declaring it does not acquire it in graphics-disabled configurations.
FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG dc46d3f8129712e42856c20e99a604a3b08ad581)
# Immutable libigl v2.5.0; other viewer recipes retain their upstream pins.
FetchContent_Declare(libigl
  GIT_REPOSITORY https://github.com/libigl/libigl.git
  GIT_TAG fdaac01bcc52888994f7afd029dcc045dd408484)
set(LIBIGL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LIBIGL_BUILD_TUTORIALS OFF CACHE BOOL "" FORCE)
set(LIBIGL_INSTALL OFF CACHE BOOL "" FORCE)
set(LIBIGL_USE_STATIC_LIBRARY OFF CACHE BOOL "" FORCE)
set(LIBIGL_OPENGL ${GM_BUILD_VIEWER} CACHE BOOL "" FORCE)
set(LIBIGL_GLFW ${GM_BUILD_VIEWER} CACHE BOOL "" FORCE)
set(LIBIGL_IMGUI ${GM_BUILD_VIEWER} CACHE BOOL "" FORCE)

# Populate these headers before libigl's recipes try single-argument
# FetchContent_Populate. libigl still defines the corresponding targets.
FetchContent_MakeAvailable(eigen)
if(GM_BUILD_VIEWER)
  # Use the font header without configuring the repository's ImGui build.
  FetchContent_Declare(libigl_imgui_fonts
    GIT_REPOSITORY https://github.com/libigl/libigl-imgui.git
    GIT_TAG 7e1053e750b0f4c129b046f4e455243cb7f804f3
    SOURCE_SUBDIR gm-header-only)
  FetchContent_MakeAvailable(libigl_imgui_fonts)
endif()
FetchContent_MakeAvailable(libigl)
