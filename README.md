# GM2627 - Geometric Modeling assignments

Student course repository: [GM2627-students](https://github.com/enricopuppo/GM2627-students.git).

This repository is intended to contain all four assignments, their handouts,
input data, supplied applications and public numerical checks. Read the common
[SETUP.md](SETUP.md) first: it explains installation, configuring the shared build
tree, downloading dependencies and building a single assignment target.

## Assignments

1. [Mesh basics, shading and sqrt(3) subdivision](assignments/a01-mesh-basics/README.md) - runnable A01 starter, handout, viewer and public checks available for review.
2. [Implicit MLS reconstruction](assignments/a02-reconstruction/README.md) - forthcoming.
3. [Discrete differential geometry and smoothing](assignments/a03-ddg/README.md) - forthcoming.
4. [Geodesic graphs and distances](assignments/a04-geodesics/README.md) - forthcoming.

This is the instructor-review version of the student distribution. A01 contains
the complete computational exercise and supplied application; course administration
and teaching-release checks remain pending. A02-A04 contain placeholder READMEs
only and have no runnable targets. Later releases will add material
to this same repository, rather than requiring another clone or per-assignment
build directory. Shared Eigen/libigl dependencies are cached once in the student
build tree and reused across assignment targets.

## Quick start

Clone once when the instructor publishes this repository. If you already have a
local checkout containing these files, start with the configure command:

```sh
git clone https://github.com/enricopuppo/GM2627-students.git
cd GM2627-students
cmake --preset student
cmake --build --preset student --target a01_skeleton_viewer --parallel 4
```

See [A01 instructions](assignments/a01-mesh-basics/README.md) for platform-specific
launch commands and public checks. Students implement computational functions in
`skeleton/`; the supplied application owns loading, rendering, controls and export.

## Course instructions

Handouts specify the required mathematical tasks and evidence. Final submission
channel, deadlines, report format and course conduct/code-assistance policy have
not yet been published here. The instructor will add them to this repository
before teaching release; this pilot introduces no new policy.
