# Roadmap

The roadmap is milestone-gated. Each milestone requires explanation,
verification, known weaknesses, and maintainer approval before the next one
starts.

## Phase 0: Foundation

- Project charter and principles
- Design decision log
- C++23/CMake baseline
- Minimal real CLI surface
- C++ test harness and first tests

## Phase 1: Repo Brief

- Architecture boundary enforcement
- Benchmark, logging, tracing, and memory policy foundations
- Safe repository traversal
- Ignore handling
- File classification
- Deterministic one-page repository brief
- First performance baseline

## Phase 2: Repo-Owned Map

- Draft repo map schema
- Declaration file design
- Verifier that compares declared structure with repository evidence
- Human-readable and machine-readable renderers

## Phase 3: Impact Evidence

- Git diff ingestion
- Changed-file impact graph
- Related test and owner hints
- Evidence explanations for every inclusion

## Phase 4: Context Compilation

- Task-aware context packing
- Token-budgeted output
- Explainable ranking
- Output adapters for common AI tools without making any vendor required

## Phase 5: Scale And Compatibility

- Incremental indexing
- Large-repository benchmarks
- Stable schema compatibility policy
- Extension interface for language and build-system adapters
