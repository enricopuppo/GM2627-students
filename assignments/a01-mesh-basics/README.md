# Assignment 1 - Mesh basics, shading and sqrt(3) subdivision

Read the [common setup guide](../../SETUP.md) once for installation and the shared
build tree. Read the [handout PDF](handout/A01.pdf) or [handout source](handout/A01.md)
for tasks, experiments and required evidence. Detailed function contracts and
allowed library calls are in [a01.hpp](include/gm/a01.hpp) and the skeleton comments.
The [classroom slides](../../slides/L1-Assignment1-Cpp.pdf) introduce Eigen,
libigl and the subdivision implementation.
Student repository: [GM2627-students](https://github.com/enricopuppo/GM2627-students.git).

## Build and launch A01 only

From the repository root:

```sh
cmake --preset student
cmake --build --preset student --target a01_skeleton_viewer --parallel 4
```

Launch on Linux:

```sh
./build/student/bin/a01_skeleton_viewer
```

On macOS:

```sh
./build/student/bin/a01_skeleton_viewer.app/Contents/MacOS/a01_skeleton_viewer
```

On Windows with Visual Studio:

```powershell
.\build\student\bin\Debug\a01_skeleton_viewer.exe
```

This uses the same build tree and dependency cache as other course assignments.
Only edit `assignments/a01-mesh-basics/skeleton/a01.cpp`; leave the shared API,
validation and supplied application intact. The untouched starter opens a cube
with explicitly labeled teacher preview normals. Compute shading, Print adjacency
and Refine report the relevant TODO until implemented. Errors retain the current
valid mesh; a preview is not evidence that your normal task is implemented.

## Inputs and controls

Load cube.obj, sphere.obj and bunny-small.off using Dataset / Load dataset, or
enter an OBJ/OFF path and choose Load path. Canonical inputs and their hashes are
in the root `data/` directory. Toggle wireframe; choose Flat, Vertex or Corner,
set the crease angle in degrees. Changing mode or crease angle immediately
recomputes shading and redraws the mesh. Compute shading remains available to
explicitly compute the current settings again. Unimplemented student routines
report their TODO and preserve the previous valid display.

Print adjacency writes to the launch terminal. Refine current performs one to
four full iterations, or a single intermediate Split / Split + relocate step.
Reset restores the original input. Export mesh writes OFF to the output path
(default `a01-result.off`). Rotate with the mouse and scroll to zoom. Capture
screenshots with your operating system, labeling model, mode and iteration.
For the subdivision exercise, use watertight (closed), consistently oriented
triangular manifold meshes without isolated vertices. Boundary handling is not a
student task; the supplied validation rejects unsupported inputs as explained in
the computational interface.

## Public numerical checks and batch operations

A01 provides three executables sharing your computational implementation:

| Target | Purpose and output |
|---|---|
| `a01_skeleton_viewer` | Interactive mesh display, shading/refinement controls and OFF export. Available with graphics enabled. |
| `a01_skeleton_batch` | Loads a mesh and runs a named operation in the terminal. Prints counts, adjacency lists or normal-field row counts; subdivision can write an OFF file. |
| `a01_public_checks` | Checks small test meshes. Prints `Public checks passed` and returns zero on success, or reports the first failure/TODO and returns nonzero. |

All entry points are supplied by the teacher. Build only the checks and batch tool
when you need them:

```sh
cmake --build --preset student --target a01_public_checks a01_skeleton_batch --parallel 4
./build/student/bin/a01_public_checks
./build/student/bin/a01_skeleton_batch data/cube.obj load
./build/student/bin/a01_skeleton_batch data/bunny-small.off adjacency
./build/student/bin/a01_skeleton_batch data/cube.obj normals
./build/student/bin/a01_skeleton_batch data/cube.obj subdivide 2 result.off
```

The final command performs two full refinement steps and writes `result.off` in
the current directory; load that file later in the viewer. The `normals` operation
prints dimensions only, not normal values or a normal-field file. In the untouched
starter, loading works and computational operations report their relevant TODO
rather than producing a successful result. Public checks intentionally fail until
the required functions are implemented.

On Windows use `bin/Debug/` and `.exe`. For graphics-disabled configuration, follow
[common setup](../../SETUP.md#numerical-work-without-graphics); use the same build
tree. CTest's `starter_todo` checks the untouched release and should be excluded after you begin
implementation (`ctest --preset student -E starter_todo`); build its test executable
first if you choose to run release checks.

Prepare the evidence listed in the handout. Follow the [submission guide](../../SUBMISSION.md)
for your private repository, assignment updates and combined submission before the final exam.
