# Memory Strategy

RepoLens treats memory usage as a product constraint, not an implementation
detail. Repository analysis must stay predictable on large repositories and must
avoid hidden process-wide allocation behavior.

## Strategy

RepoLens uses three allocation styles:

1. Ordinary RAII containers for small, long-lived configuration and domain
   values.
2. Explicit scan-local allocation for short-lived repository analysis data.
3. Streaming or chunked processing for large file contents and rendered output.

RepoLens does not replace the global allocator.

## Scan-local allocation

Repository traversal and future fact extraction will create many small objects:
paths, classification records, import edges, symbol names, and evidence records.
Those objects share a natural lifetime: a scan, an index build, or a render pass.

For those workloads, RepoLens will prefer `std::pmr` resources owned by the use
case that starts the work. The intended default is a scan-local monotonic
resource for temporary data, with explicit promotion into stable owned storage
only when facts must outlive the scan.

## Ownership rules

- Public API values must either own their data or document a clear caller-owned
  lifetime.
- Borrowed `std::string_view` and `std::span` values must not cross asynchronous
  or persisted boundaries.
- Core domain facts should use stable identifiers rather than duplicate large
  strings when repeated values become common.
- Large file contents must not be retained by default.

## Measurement

Every repository-analysis milestone must report memory behavior when practical:

- input file count
- input byte count
- peak resident memory when available
- important internal counts, such as stored paths or facts

Platform-specific peak-memory measurement belongs in infrastructure adapters,
not the domain core.
