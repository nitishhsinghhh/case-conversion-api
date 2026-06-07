# High Level Design (HLD)

## Overview

The Case Conversion Platform is a polyglot text-processing system designed around a native C++17 execution engine integrated with a managed .NET 8 orchestration layer through a custom C-style ABI boundary.

The architecture demonstrates how performance-sensitive workloads can be executed in native code while leveraging modern managed infrastructure for API lifecycle management, authentication, observability, deployment automation, and cross-platform compatibility.

The platform is intentionally structured as a layered architecture where each subsystem owns a distinct responsibility and communicates through clearly defined contracts.

---

## Architectural Goals

The primary design goals are:

* High-performance native execution
* Deterministic memory ownership
* Safe managed/unmanaged interoperability
* Cross-platform deployment support
* Production-grade observability
* Horizontal scalability
* Infrastructure automation
* Extensible processing pipelines

---

## System Architecture

```text
                    ┌─────────────────────┐
                    │    React Frontend   │
                    │   Vite + TypeScript │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │        NGINX        │
                    │ Reverse Proxy / LB  │
                    └──────────┬──────────┘
                               │
                ┌──────────────┼──────────────┐
                │              │              │
                ▼              ▼              ▼

      ┌────────────────┐ ┌────────────────┐ ┌────────────────┐
      │ ASP.NET API #1 │ │ ASP.NET API #2 │ │ ASP.NET API #N │
      └───────┬────────┘ └───────┬────────┘ └───────┬────────┘
              │                  │                  │
              └──────────┬───────┴───────┬──────────┘
                         ▼
             ┌──────────────────────────┐
             │ Dynamic P/Invoke Layer   │
             └─────────────┬────────────┘
                           │
                           ▼
             ┌──────────────────────────┐
             │  Native C++17 Engine     │
             └──────────────────────────┘
```

---

## Component Breakdown

### 1. Native Execution Engine (C++17)

The native execution engine is the computational core of the platform.

Its responsibility is to perform text-processing operations while maintaining deterministic memory behavior and minimal runtime overhead.

#### Responsibilities

* String transformation
* Spell checking
* Strategy execution
* Native memory management
* Error handling
* Buffer allocation

#### Design Patterns

##### Strategy Pattern

Encapsulates individual conversion algorithms behind a common interface.

Examples:

* Upper Case
* Lower Case
* Title Case
* Spell Check

This allows new algorithms to be introduced without modifying existing processing logic.

##### Factory Pattern

Responsible for creating the appropriate processing strategy based on runtime input.

Benefits:

* Centralized object creation
* Reduced coupling
* Simplified extensibility

##### RAII

Resource Acquisition Is Initialization is used throughout the native layer to ensure deterministic cleanup of unmanaged resources.

Benefits:

* Automatic resource release
* Exception safety
* Leak prevention

---

### 2. Interoperability Layer (C ABI Boundary)

The interoperability layer forms the bridge between managed and unmanaged execution environments.

Since .NET cannot directly consume C++ classes, a stable C-style ABI is exposed through exported functions.

#### Responsibilities ABI

* ABI stability
* Ownership transfer
* Marshaling compatibility
* Platform independence

### Exported Contract

```cpp
extern "C"
{
    char* processStringDLL(...);

    void freeMemoryDLL(char*);
}
```

### Design Goals

* Deterministic ownership transfer
* Cross-platform compatibility
* Allocator isolation
* Runtime safety

---

### 3. Managed Gateway (.NET 8)

The ASP.NET Core layer acts as the orchestration boundary between external clients and native execution.

This layer focuses on operational concerns rather than computation.

#### Responsibilities .Net Gateeway

* Authentication
* Authorization
* Validation
* Native invocation
* Error translation
* Telemetry generation
* API lifecycle management

#### Runtime Services

Shared infrastructure components are registered as Singleton services:

* Native Library Loader
* Configuration Provider
* Telemetry Provider
* Runtime Coordinators

Benefits:

* Reduced allocations
* Centralized lifecycle control
* Thread-safe reuse

#### Dynamic P/Invoke

The native library is loaded dynamically at runtime.

Benefits:

* Platform portability
* Runtime flexibility
* OS-specific binary resolution

---

### 4. Frontend Layer

The frontend provides a lightweight interface for interacting with the processing engine.

### Technology Stack

* React
* TypeScript
* Vite

#### Responsibilities Frontend

* User interaction
* API communication
* Result rendering
* Input validation

##### Design Goals (Frontend)

* Type safety
* Fast development cycles
* Low build latency
* Responsive UI

---

## 5. Load Balancing Layer

NGINX functions as the system's Layer 7 reverse proxy.

### Responsibilities (Load Balancing)

* Request routing
* Load balancing
* Failover
* Traffic distribution

#### Benefits

* Horizontal scalability
* Improved reliability
* Reduced backend contention
* Graceful instance failure handling

---

### 6. Observability Layer

The platform integrates distributed tracing using OpenTelemetry.

### Components

* OpenTelemetry SDK
* OTLP Exporter
* Jaeger

#### Responsibilities (Observability)

* Trace collection
* Request correlation
* Error tracking
* Performance analysis

#### Trace Flow

```text
Client Request
      │
      ▼
ASP.NET Core
      │
      ▼
Native Interop
      │
      ▼
C++ Execution
      │
      ▼
Jaeger Trace
```

This provides complete visibility across managed and unmanaged execution paths.

---

### 7. Deployment Architecture

The platform is deployed using Docker-based immutable infrastructure.

#### Deployment Principles

Build Once, Promote Anywhere

Artifacts are generated a single time during CI/CD execution and promoted unchanged across environments.

#### Benefits (Deployment)

* Reproducibility
* Environment consistency
* Reduced deployment risk
* Simplified rollback procedures

---

## Request Lifecycle

```text
Client
  │
  ▼
React UI
  │
  ▼
NGINX
  │
  ▼
ASP.NET Core
  │
  ▼
Validation
  │
  ▼
Native P/Invoke
  │
  ▼
C++ Engine
  │
  ▼
Processing Strategy
  │
  ▼
Result
  │
  ▼
JSON Response
  │
  ▼
Client
```

---

## Key Architectural Characteristics

| Characteristic   | Implementation                    |
| ---------------- | --------------------------------- |
| Performance      | Native C++17 execution            |
| Extensibility    | Strategy + Factory patterns       |
| Memory Safety    | RAII + ownership contracts        |
| Interoperability | Stable C ABI                      |
| Scalability      | NGINX + multiple API instances    |
| Observability    | OpenTelemetry + Jaeger            |
| Deployment       | Docker + CI/CD                    |
| Portability      | Windows, Linux, macOS             |
| Reliability      | Sentinel error model + validation |

The architecture prioritizes clear ownership boundaries, deterministic resource management, and operational simplicity while providing a foundation that can evolve beyond simple case conversion into a broader text-processing platform.
