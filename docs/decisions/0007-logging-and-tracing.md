# 0007: Logging and tracing

Status: Accepted

## Context

Repository traversal, ignore matching, classification, rendering, and future
adapter work need diagnostics and timing data. A global logger would make tests
and embedding harder, while a third-party logging dependency is premature.

## Decision

RepoLens provides a lightweight `support` logging and tracing API:

- `Logger` is an explicit object passed to subsystems that need diagnostics.
- `LogSink` is an interface for destinations.
- `StreamLogSink` writes human-readable diagnostics to an output stream.
- `TraceSpan` records elapsed wall time for scoped operations and logs on
  destruction.

The logging system lives in `repolens_support`, below the domain core. It does
not know about repositories, adapters, command-line parsing, filesystems, or AI
integrations.

## Consequences

- Tests can inject local sinks without mutating process-wide state.
- Future CLI, CI, and benchmark adapters can choose their own sinks.
- Tracing is suitable for subsystem timing but is not a substitute for the
  benchmark executable.
- The initial stream sink is intentionally simple; structured sinks require a
  separate design decision.
