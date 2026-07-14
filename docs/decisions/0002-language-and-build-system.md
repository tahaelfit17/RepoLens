# 0002: C++23 and CMake

Status: Accepted

## Context

RepoLens is intended to be a long-lived local infrastructure tool. The user
requires modern C++23 and CMake.

## Decision

RepoLens production code and tests will be written in modern C++23. CMake is the
only supported build system.

## Consequences

- The core can ship as a native, dependency-light binary.
- Build and test automation must not depend on Python, Node.js, or shell scripts.
- Cross-platform behavior must be expressed through CMake and isolated C++ code.
- External dependencies require an explicit design decision.
