# 0003: Clean Architecture

Status: Accepted

## Context

RepoLens needs to survive changing output formats, AI integrations, operating
systems, parsers, storage backends, and repository hosts. Binding domain logic to
any one interface would make the project fragile.

## Decision

RepoLens will use Clean Architecture. Stable repository concepts live in the
domain core. Use cases live in an application layer. CLI, CI, service protocols,
storage, and parser implementations are adapters or infrastructure.

## Consequences

- Core code must not depend on command line parsing, file systems, network
  services, GitHub, editors, or AI vendors.
- Adapters may be replaced without rewriting domain behavior.
- Tests should favor domain and application behavior over adapter internals.
