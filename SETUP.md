# Common setup for all assignments

This is the single installation and build guide for
[GM2627-students](https://github.com/enricopuppo/GM2627-students.git).
Each assignment's README contains its own executable names, inputs, controls and
checks; installation instructions are not duplicated inside assignments.

## Clone and locate the project root

Clone once when the instructor publishes the course material:

```sh
git clone https://github.com/enricopuppo/GM2627-students.git
cd GM2627-students
```

If you already have a local checkout containing these files, skip cloning and
configure from that checkout. The root README lists available assignments and
links to their instructions. Students receive this repository link, with
instructions and assignment material together.
No access to the private instructor repository is required.

The project root contains `README.md`, this `SETUP.md`, `CMakeLists.txt`,
`CMakePresets.json`, `cmake/`, `common/`, `data/` and `assignments/`.
Run all commands below from that root, not an assignment or handout directory.
The assignment subdirectory alone is not a complete buildable project.

## Install prerequisites

Install Git, CMake 3.24 or newer and a C++17 compiler. Intended compiler baselines
are Apple Clang 14+, GCC 11+, or Visual Studio 2022 with the Desktop C++ workload.
These are intended baselines, not a claim that every version has been tested.

- macOS: install Xcode Command Line Tools (`xcode-select --install`) and CMake.
- Linux: install CMake, a compiler, make or Ninja, and OpenGL/X11 development packages. On Ubuntu the development packages are `build-essential libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev`.
- Windows: install Git, CMake and Visual Studio's Desktop C++ workload; use a Visual Studio Developer PowerShell.

Network access is needed on the first configure to download pinned dependencies.
Eigen and libigl are supplied automatically; do not install another version by
hand. Python is not needed to compile or run the assignments.

## Configure the shared student build tree

```sh
cmake --preset student
```

The current preset uses `build/student/` as the **one shared student build tree**,
with dependencies under `build/student/_deps/`. It is shared by assignment targets,
not recreated for each assignment. `build/` and nested build directories are
ignored by Git. Do not run a separate configure inside each assignment directory.

Dependencies are declared once at project level. Their pinned sources and compiled
library targets are reused by all assignments that need them. Downloads happen at
configure time before compilation; subsequent configurations/builds reuse the cache
unless a dependency is missing or its pin changes. Keep the build tree between
assignments to retain that cache. Deleting it also deletes its downloaded libraries.

New assignments will be added to this same repository as they are released.
Consult the root README for availability and each assignment README for its
current targets and any additional requirements.

## Build only the assignment you are working on

Use the target names listed in that assignment's README. In this command,
replace `TARGET_NAME` with the exact target you want to build; it is a placeholder,
not a target supplied by CMake:

```sh
cmake --build --preset student --target TARGET_NAME --parallel 4
```

This builds the selected executable and its prerequisites, without building
unrelated assignment applications. Repeating the command compiles only changed
files and affected dependencies. Switching assignments means selecting another target in
the same build tree; it does not require another dependency download or build root.
Omitting `--target` builds all default targets currently enabled, so use an explicit
target for assignment-specific work.

Presets select Debug for both single-configuration and multi-configuration
generators. The default generator is native. If Ninja is installed, optionally add
`-G Ninja` on the first configure; do not change generators in an existing tree.
Use a compiler-specific build tree if changing compiler or architecture; the shared
tree policy applies within one compatible toolchain configuration.

## Locate and launch executables

Run from the repository root so relative input/output paths are predictable.
Assignment READMEs give exact commands. With the current preset:

- Linux: `build/student/bin/<target>`.
- macOS viewers: `build/student/bin/<target>.app/Contents/MacOS/<target>`; batch tools and checks are plain executables under `bin/`.
- Windows with Visual Studio: `build/student/bin/Debug/<target>.exe`.

Launching a viewer from a terminal keeps diagnostic and computational output visible.
The supplied application handles rendering, loading, controls and export. Students
edit computational implementations in `assignments/<id>/skeleton/` only.

## Numerical work without graphics

Graphics are optional for compiling and checking the computational code. Students
work on the **same files in skeleton/** in either configuration; they do not keep
separate graphical and numerical implementations. Both configurations call the
same computational API. Every application entry point (`main`), including the
viewer, batch runner and public checks, is supplied by the teacher. Students do
not write or modify any of these entry points.

An assignment can supply several executables using the same computational code.
Its README lists the available targets, arguments, inputs and output formats:

| Program type | Purpose |
|---|---|
| Viewer | Interactive display, controls and visual inspection. Requires a graphics-enabled build. |
| Batch runner, where supplied | Runs computational operations from the terminal. Its assignment README specifies printed output and any file exports. |
| Public numerical checks | Checks computed results without opening a window. Reports success or a failure/TODO diagnostic and returns a corresponding exit status. |

With graphics enabled (the default), numerical executables remain available.
Disabling graphics omits viewer targets and graphical dependencies; numerical
programs can still be built and run. This is useful on a machine without a display
or graphics libraries. Any visual evidence required by an assignment still needs
a graphics-enabled build.

To configure without graphics, use the same student build tree:

```sh
cmake --preset student -DGM_BUILD_VIEWER=OFF
```

Then build the numerical targets listed in the assignment README using the
`cmake --build` command above, and run them with that assignment's documented
arguments. Loading an input works in the untouched starter; operations that need
unfinished computational functions report `not implemented`. Public progress
checks may fail until the relevant TODOs have been completed.

Adjust executable paths as described above on Windows. A fresh graphics-disabled
configure acquires only computational dependencies; it does not download GLFW,
OpenGL loaders or UI libraries. Previously cached viewer libraries can remain in
the shared tree. An old viewer executable may also remain on disk after disabling
its target; it is not rebuilt by that configuration.

To enable graphics again without changing student source files or creating a
second dependency cache:

```sh
cmake --preset student
```

Then build the viewer target listed in the assignment README.

Follow each assignment README's check instructions. Public progress checks assess
your computational implementation; untouched-starter checks verify the supplied
starter before you begin editing. Run the appropriate checks for your stage of
work. Use CTest commands, filters or labels only as documented for that assignment.

## Troubleshooting

- Missing `CMakePresets.json`: change to the repository root.
- Failed dependency download: check network access, then rerun configure in the same tree.
- Missing executable: build that exact target and check platform-specific paths above.
- OpenGL/display unavailable: use the graphics-disabled configuration for numerical work.
- Viewer reports `not implemented`: complete the indicated computational TODO; do not edit viewer integration.

For task requirements, evidence, submission instructions and course conduct rules,
consult the repository README and the relevant assignment material. Publication
must supply any remaining course-specific instructions in this repository.
