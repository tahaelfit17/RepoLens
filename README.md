# RepoLens

> Repository-owned intelligence for humans, CI, and AI agents.

RepoLens makes repository structure **explicit, deterministic, explainable, and portable**.

Instead of every AI tool rediscovering your repository from scratch, RepoLens lets the repository publish its own machine-readable understanding.

> 🚧 **Early Development**
>
> RepoLens is currently in **Phase 0**.
> The public architecture, engineering standards, and project foundation are complete.
> Repository analysis features will be implemented incrementally starting with Phase 1.

---

## Why?

Modern repositories are consumed by many tools:

- AI coding agents
- IDEs
- CI pipelines
- Documentation generators
- Search and indexing systems

Today, every tool builds its own understanding independently.

RepoLens aims to provide a **single repository-owned source of truth** that any tool can consume.

---

## Current Features

Current executable:

```sh
repolens --help
repolens --version
```

Current repository includes:

- ✅ Clean Architecture foundation
- ✅ Modern C++23 + CMake
- ✅ Engineering documentation
- ✅ Architecture Decision Records (ADRs)
- ✅ Benchmark infrastructure
- ✅ Logging and tracing foundation

---

## Principles

- Repository-owned truth
- Local-first
- Deterministic
- Explainable
- Vendor-neutral
- Useful without AI
- Useful to every AI

---

## Build

Requirements:

- CMake 3.25+
- C++23 compiler

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## Roadmap

- ✅ Phase 0 — Foundation
- 🟡 Phase 1 — Repository Traversal
- ⬜ Phase 2 — Lens Pack Format
- ⬜ Phase 3 — Task-aware Context Packs
- ⬜ Phase 4 — Diff Intelligence

---

## Documentation

- Architecture: `docs/ARCHITECTURE.md`
- ADRs: `docs/decisions/`

---

## License

Apache License 2.0
