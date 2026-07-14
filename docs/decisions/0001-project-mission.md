# 0001: Project mission

Status: Accepted

## Context

Modern code tools and AI coding agents repeatedly rediscover repository
structure from raw files. That discovery is expensive, inconsistent, and often
owned by a specific vendor surface.

## Decision

RepoLens will make repository structure explicit, verifiable, portable, and
cheap to consume. The project will optimize for repository-owned truth rather
than vendor-owned context.

## Consequences

- RepoLens must stay neutral across code hosts, editors, and AI vendors.
- RepoLens must be useful without AI.
- RepoLens must provide outputs that can be reviewed in normal pull requests.
- RepoLens should avoid features that turn it into an assistant or IDE.
