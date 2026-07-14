# Performance

RepoLens measures first and optimizes second.

## Benchmark executable

`repolens_benchmarks` emits `repolens-benchmark-v1`:

```text
# repolens-benchmark-v1
# name	samples	operations_per_sample	min_ns	median_ns	mean_ns	max_ns
core_version	50	1000	...
```

Run benchmarks from a release build:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target repolens_benchmarks
build/repolens_benchmarks --output current.rbench
```

Compare against a baseline:

```sh
build/repolens_benchmarks \
  --baseline baseline.rbench \
  --max-regression-percent 10
```

## Methodology

Every performance-sensitive milestone must document:

- operating system and filesystem
- compiler and build type
- CPU model when available
- dataset or fixture
- cold-cache or warm-cache assumption
- wall-clock timing
- memory usage when practical

Every major subsystem must be measurable before it is optimized. New subsystems
must include at least one of:

- a benchmark case in `repolens_benchmarks`
- a trace span around the subsystem boundary
- a documented measurement reason explaining why the subsystem is not yet
  performance-sensitive

Phase 1 subsystem names:

- `startup`
- `filesystem.traversal`
- `ignore.matching`
- `file.classification`
- `brief.rendering`
- `output.writing`

## Initial targets

Phase 1 repository traversal targets:

- startup for `--help` and `--version`: under 50 ms warm start
- small repo brief, up to 1,000 files: under 200 ms warm cache
- medium repo brief, up to 50,000 files: under 2 seconds warm cache
- metadata-only traversal: at least 50,000 files per second on a Linux SSD
- memory for a 100,000-file metadata scan: under 128 MiB

These targets are provisional until measured on controlled Linux hardware.
