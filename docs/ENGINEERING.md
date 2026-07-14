# Engineering Standards

## Language

Production code and tests use modern C++23. The project avoids scripting
languages in the build and test path.

## Build system

CMake is the only supported build system. Build behavior belongs in CMake files,
not in shell scripts.

## Tests

Tests are C++ binaries registered with CTest. Each milestone must include tests
for committed behavior. Future broad behavior will add fixture repositories and
golden outputs only when those outputs are stable compatibility contracts.

## Design decisions

Every significant architecture, governance, dependency, format, compatibility,
or performance decision must be recorded in `docs/decisions`.

## Performance discipline

Performance-sensitive work must define:

- the dataset or fixture being measured
- the command being measured
- cold-cache or warm-cache assumptions
- wall-clock time and peak memory when practical

Phase 0 has no repository-analysis benchmark because no repository-analysis
behavior exists yet.

## No placeholders

Code merged to main must either implement committed behavior or not exist.
Skeleton files without executable purpose are not allowed.

## Error handling

User-facing commands should fail with concise messages and stable nonzero exit
codes. Library code should prefer explicit value types and narrow exceptions at
process boundaries.
