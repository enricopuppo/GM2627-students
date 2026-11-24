# GM2627 - Geometric Modeling assignments

Course repository: [GM2627-students](https://github.com/enricopuppo/GM2627-students.git).

This repository supplies the assignments, handouts, input data, teacher-provided
applications and public numerical checks. **The first release contains A01 only.**
The instructor will disclose A02-A04 progressively in this same repository.

## Start here

1. Follow [the Git and submission guide](SUBMISSION.md) to clone once, create your
   independent private homework repository and grant the instructor access.
2. Follow [SETUP.md](SETUP.md) to install prerequisites, configure the shared build
   tree and build the assignment target you need.
3. Each time the instructor discloses a new assignment, follow the
   [update procedure](SUBMISSION.md#3-update-whenever-a-new-assignment-is-disclosed).
   It preserves your existing solutions and adds the new starter.
4. Submit **all four assignments together before your final exam**, using your
   private repository URL and the full submitted commit hash, as described in
   [the submission procedure](SUBMISSION.md#4-submit-all-assignments-together-before-the-final-exam).
   The instructor will announce the exact cutoff for your exam session.

## Assignments

1. [Mesh basics, shading and sqrt(3) subdivision](assignments/a01-mesh-basics/README.md) - initial release.
2. Implicit MLS reconstruction - forthcoming.
3. Discrete differential geometry and smoothing - forthcoming.
4. Geodesic graphs and distances - forthcoming.

The [A01 classroom slides](slides/L1-Assignment1-Cpp.pdf) introduce Eigen, libigl
and the subdivision implementation. Use the handout for requirements and the
header/skeleton comments for exact function contracts.

Later releases will add the corresponding assignment links and material. A
placeholder directory does not make an assignment available. You use one checkout
and one `build/student/` directory for the whole course. CMake fetches the pinned
dependencies when needed and reuses them across assignment targets.

## First build

After cloning and setting up your remotes as explained in [SUBMISSION.md](SUBMISSION.md),
run from your local repository root:

```sh
cmake --preset student
cmake --build --preset student --target a01_skeleton_viewer --parallel 4
```

See the [A01 README](assignments/a01-mesh-basics/README.md) for platform-specific
launch commands, viewer controls and public checks. Students implement
computational functions in `skeleton/`; the supplied application owns loading,
rendering, controls and export. Detailed function contracts are in the assignment
header and matching skeleton comments. Handouts specify tasks and required evidence.

Final submission uses a private GitHub repository, not a project ZIP. Keep your
solutions private and push them to your own `origin`; the course repository is
`upstream`, used to fetch releases and corrections. Course rules on collaboration,
attribution and code assistance remain in effect.
