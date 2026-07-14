# 0008: Memory allocation strategy

Status: Accepted

## Context

RepoLens must scale from small libraries to large monorepos. Repository analysis
creates many short-lived allocations, but replacing the global allocator would
make behavior harder to embed, test, and debug.

## Decision

RepoLens will not replace the global allocator. The project will use ordinary
RAII containers for small and stable data, `std::pmr` resources for explicit
scan-local lifetimes, and streaming for large contents or outputs.

The application layer owns scan lifetime. Infrastructure adapters may allocate
temporary data through caller-provided memory resources, but domain core types
must not depend on a process-wide allocator policy.

## Consequences

- Allocation behavior remains explicit and testable.
- Future traversal and language adapters can release scan-local memory in one
  operation.
- Public APIs must be careful about borrowed views and lifetime documentation.
- Memory optimization must be justified with measurements, not assumptions.
