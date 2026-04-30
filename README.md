# High-Performance String Processing: A Polyglot Architecture

This is a high-concurrency, cross-platform string processing ecosystem. It demonstrates a Native C++17 engine integrated into a .NET 8 managed environment via a custom C-style ABI. The project serves as a technical blueprint for bridging managed and unmanaged memory, implementing the Strategy and Factory patterns, and maintaining an immutable Docker-based deployment pipeline.

![C++ CI](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/native-engine-ci.yml?branch=main&label=Engine%3A%20Native&style=flat-square)
![.NET API Status](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/dotnet-tests.yml?branch=main&label=Gateway%3A%20.NET%208&style=flat-square)
![.NET API Integeration Tests](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/dotnet-integration-tests.yml?branch=main&label=Integeration%20Tests&style=flat-square)
![Full Project Build](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/native-engine-ci_Non_DLL.yml?branch=main&label=Build%3A%20Multi-OS&style=flat-square)
![Docker Push](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/docker-hub-distribution.yml?branch=main&label=Registry%3A%20Image&style=flat-square)
![Deployment](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/production-release.yml?label=Release%3A%20v2.0.0&style=flat-square)
![Quality](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/cpp-quality-guard.yml?branch=main&label=Quality%3A%20Clang&style=flat-square)
![Telemetry E2E](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/telemetry-e2e-verification.yml?branch=main&label=Observability%3A%20E2E&style=flat-square)
![Security Audit](https://img.shields.io/github/actions/workflow/status/nitishhsinghhh/case-conversion-api/security-analysis.yml?branch=main&label=Security%3A%20CodeQL&style=flat-square)

![C++](https://img.shields.io/badge/C++-17-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![.NET](https://img.shields.io/badge/.NET-8.0-512BD4?style=flat-square&logo=dotnet&logoColor=white)
![React](https://img.shields.io/badge/React-TS-61DAFB?style=flat-square&logo=react&logoColor=black)
![YAML](https://img.shields.io/badge/YAML-Config-CB171E?style=flat-square&logo=yaml&logoColor=white)
![Docker](https://img.shields.io/badge/Docker-Containerized-2496ED?style=flat-square&logo=docker&logoColor=white)
![CI/CD](https://img.shields.io/badge/CI/CD-GitHub%20Actions-2088FF?style=flat-square&logo=github-actions&logoColor=white)
![Telemetry](https://img.shields.io/badge/Telemetry-OpenTelemetry-F48420?style=flat-square&logo=opentelemetry&logoColor=white)
![Code Base](https://img.shields.io/badge/Code_Base-84k_Lines-blueviolet?style=flat-square)
![License](https://img.shields.io/github/license/nitishhsinghhh/case-conversion-api?style=flat-square&color=blue)

![Visitors](https://api.visitorbadge.io/api/visitors?path=nitishhsinghhh/CaseConversionAPI&label=Project%20Views&countColor=%232088ff&style=flat-square)

---

## Table of Contents

* [System Architecture](#system-architecture)
* [CI/CD & Deployment Pipeline](#cicd--deployment-pipeline)
* [Components](#components)
  * [1. Core Logic: Native Conversion Engine](#1-core-logic-native-conversion-engine)
  * [2. Managed Gateway: .NET REST API](#2-managed-gateway-net-rest-api)
  * [3. Presentation Layer: Modern Web Interface](#3-presentation-layer-modern-web-interface)
* [Engineering Deep Dive](#engineering-deep-dive)
  * [1. Concurrency & Thread-Safety](#1-concurrency--thread-safety)
  * [2. Design Patterns Used](#2-design-patterns-used)
  * [3. Defensive Interop Design](#3-defensive-interop-design)
  * [4. Telemetry & Observability](#4-telemetry--observability)
  * [5. Hardware-Specific Optimization (Apple M2)](#5-hardware-specific-optimization-apple-m2)
  * [6. Reliability and Fault Tolerance](#6-reliability-and-fault-tolerance)
  * [7. Memory Sovereignty and The Interop Lifecycle](#7-memory-sovereignty-and-the-interop-lifecycle)
* [Quick Start](#quick-start)
  * [Run the Load-Balanced Cluster](#run-the-load-balanced-cluster)
  * [Performance Metrics and Insights RAII](#performance-metrics-and-insights-raii)
* [Technical Significance](#technical-significance)
* [Project Vitality and Scale](#project-vitality-and-scale)
* [Project Timeline and Roadmap](#project-timeline-and-roadmap)
* [Summary](#summary)

---

## System Architecture

The architecture addresses the inherent challenges of exposing unmanaged performance logic to a managed web stack. It adheres to a Strict Separation of Concerns through three primary tiers:

* The Core: A C++17 engine utilizing the Strategy and Factory patterns for extensible string processing.
* The Bridge: A custom C-style ABI wrapper with explicit memory ownership management (allocate/free contract).
* The Gateway: A .NET 8 REST API utilizing Dynamic P/Invoke via NativeLibrary for platform-agnostic service execution.
* The UI: A type-safe React/TypeScript frontend built on Vite for sub-second developer turnaround.
* The DevOps: A multi-stage Docker orchestration supporting Artifact Promotion (Dev → Staging → Prod) to ensure environmental parity.

![alt text](assets/API.png)

---

### CI/CD & Deployment Pipeline

```mermaid
graph TD
    subgraph Local_Development
        A[M2 MacBook Air] -->|git push| B(GitHub Repository)
    end

    subgraph CI_CD_Pipeline
        B --> C{GitHub Actions}
        C --> D[C++17 Build Matrix]
        C --> E[.NET 8 Build]
        D & E --> F[Multi-Stage Docker Build]
        F --> G[GHCR.io Registry]
    end

    subgraph Production_Runtime
        G --> H[NGINX Load Balancer]
        H --> I[Backend Replica 1]
        H --> J[Backend Replica 2]
        H --> K[Backend Replica 3]
        H --> L[Backend Replica 4]
        I & J & K & L --> M[Native C++ Engine]
    end
```

[↑ Back to Top](#high-performance-string-processing-a-polyglot-architecture)

---

## Components

The architecture is divided into three distinct functional layers, each optimized for its specific role in the request lifecycle.

### 1. Core Logic: Native Conversion Engine

The engine serves as the high-performance foundation of the system, encapsulating the complex string transformation logic.

* Implementation: Developed in C++17 utilizing the Strategy and Factory patterns for modularity.
* Build System: Orchestrated via CMake to produce platform-agnostic shared binaries:
  * Windows → `libProcessStringDLL.dll`
  * macOS → `libProcessStringDLL.dylib`
  * Linux → `libProcessStringDLL.so`

### 2. Managed Gateway: .NET REST API

The API layer acts as the secure bridge between unmanaged native code and the external web environment.

* Interoperability: Utilizes P/Invoke with custom marshalling logic to invoke exported native functions.
* Interface: Exposes standardized RESTful endpoints (e.g., /api/WordCase/convert) for secure, high-concurrency consumption.
* Lifecycle: Managed through the standard dotnet CLI, supporting seamless artifact promotion to production environments.

### 3. Presentation Layer: Modern Web Interface

A type-safe, responsive interface designed for sub-second interaction and real-time feedback.

* Stack: Built with Vite + React and TypeScript to ensure strict data modeling and developer efficiency.
* Communication: Consumes the .NET REST API to deliver hardware-accelerated string transformations to the end-user.
* Optimized Delivery: Compiled via npm run build into a lightweight, static distribution (dist/) ready for edge-network hosting.

---

## Engineering Deep Dive

### 1. Concurrency & Thread-Safety

In a high-throughput REST environment, thread-safety is paramount. The integration layer has been engineered with the following principles:

* Stateless Execution: The native C++ engine is entirely Stateless. Every call to processStringDLL operates on its own stack and heap allocations, ensuring that the .NET ThreadPool can safely execute concurrent P/Invoke calls.

* Reentrancy: The library is fully reentrant. There are no global variables or shared static states within the conversion logic, eliminating the risk of race conditions or shared-state contention.

* Thread-Safe Marshalling: All data passed across the ABI boundary is deep-copied, ensuring that memory used by one thread is never modified by another.

### 2. Design Patterns Used

* Strategy Pattern: Encapsulates conversion algorithms, allowing for runtime algorithm selection.

* Factory Pattern: Decouples the client from the specific strategy implementation.

* Client Dispatcher: Manages the lifecycle of the strategy and handles the execution pipeline.

* RAII (Resource Acquisition Is Initialization): Employed in C++ to manage internal resources and in C# via IDisposable to ensure native library handles are released.

Note on Thread-Safety: The native C++ engine is designed to be Stateless and Thread-Safe, allowing the .NET pool to safely execute concurrent P/Invoke calls without shared-state contention.

### 3. Defensive Interop Design

The bridge between .NET 8 and C++17 is engineered as a "Safe Harbor." The system ensures that native failures never crash the managed process by implementing a multi-tiered error trap.

* The Sentinel Pattern: Rather than returning null pointers or throwing unhandled SEH exceptions, the engine returns Sentinel Strings (e.g., ERROR_BUFFER_OVERFLOW_LIMIT_5MB). This allows the .NET layer to perform a graceful string comparison and map the failure to a managed ArgumentException or SecurityException.

* Security Gating: A hard-coded 5MB Input Ceiling is enforced at the DLL entry point. This acts as a circuit breaker against potential Denial of Service (DoS) attacks attempting to exhaust the unmanaged heap.

### 4. Telemetry & Observability

Integrated OpenTelemetry (OTLP) for end-to-end distributed tracing. W3C Trace IDs are propagated into the C++ layer to correlate native logs with specific REST requests.

Every error returned by the native layer is tagged with the provided traceId. If the factory fails to create a strategy or an allocation fails, the error is correlated in the Jaeger/OpenTelemetry dashboard for immediate root-cause analysis.

```Bash
# Start the Jaeger collector and UI
./scripts/run-telemetry.sh start
```

* UI Dashboard: <http://localhost:16686>

* OTLP Endpoint: <http://localhost:4317> (gRPC)

### 5. Hardware-Specific Optimization (Apple M2)

* **P-Core Saturation:** `MaxDegreeOfParallelism` is explicitly set to 4. This aligns with the M2's Performance Cores, ensuring heavy C++ string transformations maintain maximum IPC (Instructions Per Cycle) without being offloaded to Efficiency Cores.

Please note that if this runs in a Docker container on an Intel Xeon or AMD EPYC server (common in Production), ProcessorCount might be 64, but our code will still cap at 4.This is scope for future enhancement.

* Double-Lock Memory Safety: - Global: 20MB batch ceiling prevents the 8GB Unified Memory from triggering SSD swap.
  * Local: 5MB native limit prevents buffer overflows in unmanaged memory.
* Contention-Free Buffering:** Utilizes `ConcurrentBag<T>` to allow parallel P-Cores to flush data back to managed memory without the lock-contention overhead of traditional `List<T>` synchronization.

### 6. Reliability and Fault Tolerance

| Scenario            | Native C++ Sentinel              | Managed .NET Response   | Architectural Significance                                  |
|-------------------- |----------------------------------|-------------------------|-------------------------------------------------------------|
| Payload > 5MB       | ERROR_BUFFER_OVERFLOW...         | 413 Payload Too Large   | Prevents heap-based DoS attacks.                            |
| Invalid Option      | ERROR_INVALID_CONVERSION...      | 400 Bad Request         | Validates Enum integrity at the ABI boundary.               |
| Null Reference      | ERROR_NULL_INPUT                 | 400 Bad Request         | Defensive guard against malformed P/Invoke calls.           |
| Heap Exhaustion     | FATAL_ALLOCATION_FAILURE         | 500 Internal Error      | Traps std::bad_alloc before process termination.            |
| Malformed ID        | ERROR_MALFORMED_TRACE_ID         | 400 Bad Request         | Protects telemetry buffers from overflow.                   |

### 7. Memory Sovereignty and The Interop Lifecycle

To achieve a "Zero-Leak" footprint under extreme concurrency (validated by the 1M request soak test), the system implements a Tiered Memory Ownership Protocol. This ensures deterministic cleanup across the ABI boundary while maintaining high-performance move semantics within the core.

#### Tier 1: Native RAII & The Rule of Five (Internal Sovereignty)

The C++ engine handles its own internal resource state using the Rule of Five. By explicitly defining Move/Copy semantics, we ensure the engine is "memory-safe by design" before a single byte ever reaches the bridge.

* Invariant Protection: Prevents double-frees and dangling pointers during high-frequency strategy rotation.

* Move Optimizations: Leverages C++17 move constructors to transfer ownership of large string buffers without redundant heap allocations.

#### Tier 2: The C-Style ABI (The Stateless Bridge)

The transition layer acts as a strict Marshalling Contract. We avoid the pitfalls of platform-specific allocators by using a standard extern "C" interface.

* Deterministic Hand-off: The engine yields a raw char* to the managed runtime, marking the precise moment ownership transfers from C++ to .NET.

* Cross-Platform Parity: Ensures identical memory layout and behavior across Clang (macOS) and GCC (Linux CI).

#### Tier 3: Managed Orchestration (Deterministic Cleanup)

The .NET 8 layer acts as the ultimate orchestrator of the memory lifecycle.

* Marshalling Integrity: .NET receives the IntPtr, marshals it into a managed System.String, and takes responsibility for the final cleanup.

* Deterministic Disposal: All native calls are wrapped in try-finally blocks (or SafeHandle patterns) to ensure the exported freeString function is invoked, preventing Resident Set Size (RSS) bloat in the containerized environment.

* Technical Significance: This hierarchy solves the "Double-Delete" problem. C++ manages the object lifetime, while C# manages the buffer lifetime after the hand-off.

Engineering Insight: This architecture eliminates the "Double-Delete" risk. C++ governs the object state, while C# governs the result buffer after the native execution context has exited.

---

[↑ Back to Top](#high-performance-string-processing-a-polyglot-architecture)

---

## Quick Start

Prerequisites

* Docker & Docker Compose

* Apple M2 (Recommended) or ARM64/x64 Linux/Windows

### Run the Load-Balanced Cluster

To support massive horizontal scaling, the system utilizes an NGINX Reverse Proxy as a Layer 7 Load Balancer. This architecture allows the API to scale beyond a single process, distributing load across multiple isolated containers.

* Dynamic Scaling: Orchestrated via Docker Compose with a replicas: 4 configuration, perfectly mapping to the M2's Performance Cores for maximum throughput.

* Health-Aware Routing: NGINX ensures traffic is only routed to "Ready" .NET instances, facilitating zero-downtime updates and maintenance.

* Performance Baseline: Validated through a 300,000-request soak test, achieving a sustained ~2,643 req/s with a 100% success rate.

* Latency Smoothing: By distributing requests, the P(95) latency is stabilized at 56ms, significantly reducing the "Tail Latency" spikes caused by parallel Garbage Collection events in managed memory.

To spin up the system with 4 backend replicas and the NGINX Load Balancer:

```Bash
docker compose -f docker-compose-load.yml up --scale backend=4 -d
```

* API Gateway: <http://localhost:8080>

* Frontend UI: <http://localhost:5173>

* Telemetry: <http://localhost:16686>

### Performance Metrics and Insights RAII

The project reached a major engineering milestone with the implementation of RAII and the Rule of Five in the native core. Below is the comparative data from a 1,000,000 request stress test conducted on Apple M2 hardware, demonstrating the massive efficiency gains in the polyglot bridge.

| Metric                 | Pre-RAII Baseline | Post-RAII (Optimized)  | Net Improvement           |
|----------------------- |-------------------|------------------------|---------------------------|
| Throughput             | 2,262.54 req/s    | 2,827.97 req/s         | +25% Gain                 |
| Mean Latency           | 44.19 ms          | 35.36 ms               | 20% Reduction             |
| Median Latency (P50)   | 38.00 ms          | 22.00 ms               | 42% Snappier              |
| Tail Latency (P99)     | 102.00 ms         | 134.00 ms              | Thermal / TCP Overhead    |
| Success Rate           | 99.996%           | 99.998%                | Near-Perfect Reliability  |

---

### Technical Significance

* Leak-Proof Architecture: Stable Resident Set Size (RSS) proves the manual memory management and RAII patterns in the C++ layer are production-grade.

* Sustained Throughput: Maintaining an average latency of 0.45ms over a quarter-million requests proves there is no performance decay or "warm-up" penalty in the native bridge.

* Hardware Efficiency: Optimized for Apple Silicon (arm64), leveraging unified memory to minimize data copy overhead during managed-to-unmanaged transitions.

---

## Project Vitality and Scale

The codebase is a significant polyglot undertaking, engineered for high-performance across multiple domains (Native, Managed, and Frontend). Below is a snapshot of the source lines of code (SLOC) excluding build artifacts and dependencies:

| Language | Files | Code (Lines) | Role in Ecosystem |
| :--- | :--- | :--- | :--- |
| **C++ / Headers** | 202 | **55,608** | High-performance Strategy Engine & Core ABI |
| **Python** | 32 | 3,839 | Performance testing & Stress-test automation |
| **YAML / Docker** | 26 | 2,781 | Multi-stage CI/CD & Cluster Orchestration |
| **CMake / Make** | 58 | 2,936 | Cross-platform Native Build System |
| **C# (.NET 8)** | 12 | 664 | High-concurrency Managed Gateway |
| **Web (TS/JS/SCSS)** | 16 | 427 | Type-safe Presentation Layer |
| **TOTAL** | **448** | **84,268** | **Core Source Lines** |

> **Engineering Insight:** While the project environment handles over 190k lines, the core logic is concentrated in ~84k lines of optimized source. The C++ dominance (approx. 66% of core) underscores the focus on raw native efficiency.

---

## Project Timeline and Roadmap

| Milestone          | Date            | Description                                                                                                                 |
|------------------- |-----------------|-----------------------------------------------------------------------------                                                |
| Project Inception  | April 4, 2026   | Project start: Designing the C-style ABI and C++17 Strategy patterns.                                                       |
| v1.0.0 Release     | April 20, 2026  | The Foundation: Stable Polyglot Architecture. M2 P-Core optimization and 250k request validation.                           |
| v2.0.0 Release     | April 23, 2026  | The "Millionaire" Milestone: Integrated NGINX Layer 7 Load Balancing and passed the 1M request endurance test.              |
| v2.1.0 Release     | April 25, 2026  | Industrial Hardening: Full OpenTelemetry integration, CI/CD pipeline domain-standardization, and CodeQL security alignment. |
| v2.2.0 Release     | April 30, 2026  | Achieved a 25% throughput increase (2,827 req/s) and 42% median latency reduction by implementing RAII.                     |

---

## Summary

Developed a cross-platform string conversion ecosystem utilizing a high-performance C++17 engine integrated into a .NET 8 microservice via P/Invoke. Engineered a Zero-Leak memory management policy across the ABI boundary and implemented a multi-stage Docker CI/CD pipeline supporting immutable artifact promotion across Dev, Staging, and Production environments.
