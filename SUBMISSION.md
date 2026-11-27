# Working with Git and submitting the assignments

Use **one independent public GitHub repository for all four assignments**.
Clone the course repository once, update your working copy each time the
instructor discloses a new assignment, and submit all assignments together
**before your final exam**. There is no separate submission at each release.
The instructor will announce the exact submission cutoff for your exam session.

## 1. First release: clone A01 and create your public repository

The initial course release contains **A01 only**. A02-A04 will be added later;
placeholder descriptions do not mean an assignment has been released.

Sign in to GitHub and create a new repository named, for example,
`GM2627-homework`. Set its visibility to **Public**. Leave it empty: do not add
a README, licence or `.gitignore`. Create an independent repository using **New
repository**, rather than forking the course repository.

Clone the student course repository into a local working directory:

```sh
git clone https://github.com/enricopuppo/GM2627-students.git GM2627-homework
cd GM2627-homework
```

Set the two remote names below. Replace `YOUR_USERNAME` with your GitHub username
and adjust the homework repository name if you chose another one:

```sh
git remote rename origin upstream
git remote add origin https://github.com/YOUR_USERNAME/GM2627-homework.git
git push -u origin main
git remote -v
```

Authenticate to GitHub when requested, using your normal GitHub Git credentials
or credential manager. The first push copies the starter and its course history
into your public homework repository. The resulting remotes must be:

| Remote | Destination | Use |
|---|---|---|
| `upstream` | `enricopuppo/GM2627-students` | Fetch assignments and instructor corrections |
| `origin` | Your public homework repository | Push your work and final submission |

If you already cloned the course repository, use that existing directory and
skip the clone command. Check `git remote -v`: rename `origin` only if it still
points to the course repository. If the two remotes are already configured as
above, do not add or rename them again.

If you already created a private homework repository, change its visibility to
**Public** and keep using the same checkout and remotes. The instructor can open
the public repository directly using its URL. Keep it public through assessment.

## 2. Develop and save your work

Follow [SETUP.md](SETUP.md) for installation and builds. Run commands from your
local repository root. Keep the same checkout and `build/student/` tree throughout
the course; later assignments reuse the dependencies already downloaded there.

Implement the designated computational functions in each assignment's
`skeleton/` directory. Leave supplied interfaces, applications, build files and
input data unchanged unless the assignment explicitly instructs otherwise.
Keep reports and required screenshots/evidence under `reports/`, organized by
assignment, for example `reports/A01/` and `reports/A02/`. Include a
`reports/README.md` index so the instructor can locate each assignment's report
and evidence. Follow each handout's content requirements.

Commit and push regularly. Review the staged file list before committing:

```sh
git status
git add -A
git diff --cached --stat
git commit -m "Implement A01 neighborhood computations"
git push origin main
```

Use a message describing your actual changes. If there are no changes to commit,
skip `git add` and `git commit`. `git add -A` stages all nonignored changes, so
check that they are intentional. Keep generated meshes, temporary output and
large intermediate results under `build/`; keep the required report/evidence
files under `reports/`. Never commit build products or downloaded libraries.
The supplied `.gitignore` excludes the shared build tree.

Push only to **your `origin`**. You do not submit implementations to the course
repository and do not open a pull request there. A local commit records your
work locally; a push copies those commits to your own public GitHub repository.

## 3. Update whenever a new assignment is disclosed

After the instructor announces each new assignment, update the same checkout.
Use the same process for announced corrections. First commit your outstanding
work as above. Run `git status` and proceed when the working tree is clean and
no merge is in progress.

```sh
git switch main
git fetch upstream
git merge --no-edit upstream/main
git push origin main
```

`fetch` downloads the instructor's commits without changing your working files.
`merge` incorporates them while keeping your own committed implementations.
For example, when A02 is added, your A01 solution remains and the new A02 starter
appears beside it. The final push saves the combined history in your public
repository. You do not clone again, replace your working directory or delete
completed assignments.

Adding a new assignment normally merges automatically. A correction that overlaps
with your changes can cause a merge conflict. If Git reports a conflict:

1. Run `git status` to identify the affected files. Resolve each conflict,
   preserving your implementation and incorporating the instructor's correction.
2. Remove the conflict markers, review the result, and stage the resolved files
   with `git add` followed by their paths.
3. Run `git merge --continue`, saving the merge message if an editor opens, then
   `git push origin main`.

If you cannot resolve the conflict, use `git merge --abort` to return to the state
before the merge and ask the instructor for help. Your previously committed work
is retained. Do not discard your solution by replacing it with a fresh starter.

After a successful update, read the new assignment README and reconfigure:

```sh
cmake --preset student
```

Then build the target named in that assignment's README. Use the existing shared
build tree. Re-run relevant checks after merging corrections.

## 4. Submit all assignments together before the final exam

Make **one final submission covering A01-A04**, including the required reports,
evidence and any optional tasks you completed. Pushing during development is a
backup/update operation; it does not by itself constitute the final submission.

Before the announced cutoff:

1. Incorporate announced course updates and run the relevant checks. Ensure all
   required computational source changes, reports and evidence are committed.
2. Confirm that `reports/README.md` identifies the material for each assignment.
   Record your OS, compiler, build commands and check results in your reports.
3. Run the following commands from the repository root:

```sh
git switch main
git status
git push origin main
git rev-parse HEAD
```

The working tree must be clean, the push must succeed, and the last command prints
the **full commit hash** identifying your submitted version. Open the repository
URL in a browser window where you are signed out of GitHub and verify that you
can view the repository and the submitted commit. Uncommitted or unpushed files
are not included.

Submit a short text entry or text file through the university submission system
containing:

```text
Name: YOUR NAME
Student number: YOUR STUDENT NUMBER
Repository: https://github.com/YOUR_USERNAME/GM2627-homework
Commit: FULL OUTPUT OF git rev-parse HEAD
Reports and evidence: reports/README.md
```

This is a repository-based submission; no project ZIP or build directory is
required. The instructor will clone your repository and check out the specified
commit. Later commits do not change the version identified by that hash. If you
revise your submission before the cutoff, the submission record must identify
the new pushed commit, subject to the university system's submission rules.

The instructor will communicate the exact deadline and any further report-format
or course-conduct requirements. This workflow does not change the course rules
on collaboration, attribution or code assistance.
