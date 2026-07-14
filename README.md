# RepoLens

RepoLens is the open convention and toolchain for repository-owned intelligence.

Its long-term purpose is to let a repository publish a neutral, deterministic,
reviewable map of itself so humans, CI systems, editors, and AI coding agents do
not need to rediscover the project from scratch.

## Current status

RepoLens is in Phase 0. The repository contains the project charter, engineering
standards, initial design decisions, and a minimal C++23/CMake foundation. Repo
analysis features begin only after the Phase 0 milestone is approved.

The current executable intentionally supports only:

```sh
repolens --help
repolens --version
```

## Project principles

- The repository owns its truth.
- Core behavior is local-first and deterministic.
- Outputs must be explainable and reviewable.
- RepoLens must remain useful without AI and useful to any AI.
- The core must stay small enough to audit and maintain.

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) and
[docs/decisions](docs/decisions) for the design record.

## Build

Requirements:

- CMake 3.25 or newer
- A C++23 compiler

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

## License

RepoLens is licensed under the Apache License 2.0.
