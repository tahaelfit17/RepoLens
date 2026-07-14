# Security Policy

RepoLens is designed to inspect repositories without executing project code.

## Supported versions

RepoLens is pre-release. Security fixes apply to the main branch until a stable
release policy is published.

## Reporting a vulnerability

Open a private security advisory when hosting is available. Until then, report
security-sensitive issues directly to the maintainers listed by the repository
owner.

## Security boundaries

- RepoLens must not execute repository build scripts, tests, package manager
  commands, or generated commands by default.
- RepoLens must treat repository contents as untrusted input.
- RepoLens must avoid uploading repository contents unless the user explicitly
  invokes a future integration that documents the data flow.
