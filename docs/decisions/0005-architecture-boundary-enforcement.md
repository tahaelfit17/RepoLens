# 0005: Architecture boundary enforcement

Status: Accepted

## Context

Phase 0 documented Clean Architecture, but documentation alone does not prevent
layer drift. RepoLens needs mechanical checks before repository traversal,
language adapters, and future integrations increase the number of modules.

## Decision

RepoLens enforces architecture boundaries in two ways:

- CMake targets express intended dependencies.
- `cmake/check_architecture.cmake` scans source includes and fails CTest when a
  lower layer includes a higher layer.

The current layer order is:

```text
support -> core -> application -> infrastructure/adapters -> apps
```

`support` contains cross-cutting utilities that do not know about RepoLens
domain concepts. `core` may use `support`, but it must not depend on CLI,
infrastructure, adapters, future integrations, direct filesystem access, or
process interaction.

## Consequences

- Architecture violations are test failures, not review comments.
- New layers must update the boundary checker before their code can be relied
  upon.
- The checker is intentionally conservative and may reject includes that need a
  design decision before they are allowed.
