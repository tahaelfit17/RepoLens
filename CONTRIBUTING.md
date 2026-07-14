# Contributing to RepoLens

RepoLens is intended to be a long-term open-source infrastructure project. The
project favors small, reviewable changes with explicit design reasoning.

## Engineering expectations

- Use modern C++23 for production code and tests.
- Use CMake as the build system.
- Keep dependencies out of the core unless an accepted design decision explains
  why they are necessary.
- Add or update tests with behavior changes.
- Add or update a design decision for meaningful architecture, governance,
  format, dependency, or compatibility changes.
- Prefer deterministic behavior over heuristic convenience.
- Do not add placeholder implementations.

## Commit shape

Commits should be small enough for reviewers to understand in one sitting. A
good commit has one reason to exist: a design decision, a focused behavior
change, a test-only change, or a mechanical cleanup.

## Certificate of Origin

Contributions use the Developer Certificate of Origin. Sign commits with:

```sh
git commit -s
```

The sign-off states that you have the right to contribute the change under the
project license.
