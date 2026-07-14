# 0006: Benchmark infrastructure

Status: Accepted

## Context

RepoLens must measure major subsystems before optimizing them. Shell scripts or
ad hoc timing commands would make measurements hard to reproduce across
platforms.

## Decision

RepoLens provides a dedicated C++ benchmark executable, `repolens_benchmarks`.
It runs registered benchmark cases, emits a stable tab-separated
`repolens-benchmark-v1` format, and can compare current results against a prior
baseline with a configurable median-regression threshold.

Benchmarks are built with CMake through `REPOLENS_BUILD_BENCHMARKS`.

## Consequences

- Benchmark behavior is versioned source code, not external scripting.
- Regression detection compares median nanoseconds per operation.
- Benchmarks can be smoke-tested by CTest, while serious performance gates can
  use controlled baseline files on stable machines.
- Benchmark outputs are engineering diagnostics, not public Lens Pack formats.
