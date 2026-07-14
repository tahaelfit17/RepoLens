# Architecture

RepoLens is structured as a Clean Architecture project. Dependencies point
inward toward stable domain concepts and outward toward replaceable interfaces.

## Layers

### Domain core

The core owns stable concepts that do not depend on the file system, command
line parsing, CMake, GitHub, editors, network services, or AI providers.

Current Phase 0 core:

- `repolens::core::Version`
- `repolens::core::version()`
- `repolens::core::product_name()`

### Application layer

The application layer will own use cases such as scanning a repository,
constructing repository facts, verifying declarations, and rendering context.
This layer may depend on the domain core but not on concrete adapters.

### Interface adapters

Adapters translate between application use cases and external interfaces:

- CLI commands
- machine-readable output formats
- future read-only service protocols
- future CI integrations

The Phase 0 CLI is an adapter over the version core.

### Infrastructure

Infrastructure code handles operating-system details, file walking, Git data,
storage, and optional parsers. Infrastructure implements interfaces required by
the application layer.

## Dependency rule

Source dependencies must point inward:

```text
apps / adapters -> application -> core
infrastructure -> application -> core
```

Core code must not include infrastructure headers or depend on concrete command
line, filesystem, network, editor, or AI vendor behavior.

## Cross-platform policy

Linux is the primary platform. Windows is a first-class secondary platform.
Platform-specific code must be isolated behind interfaces and covered by tests
where the behavior can be exercised without relying on a specific shell.

## Format policy

RepoLens will treat machine-readable outputs as compatibility contracts.
Human-readable Markdown is a renderer, not the source of truth.
