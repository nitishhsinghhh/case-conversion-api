# Case Conversion API — Design Document

## Revision History

| Version | Date       | Author       | Description                                           |
| :------ | :--------- | :----------- | :---------------------------------------------------- |
| 1.0     | 2026-04-11 | Nitish Singh | Initial Architecture & Strategy Pattern Design.       |
| 1.1     | 2026-04-14 | Nitish Singh | Added P/Invoke Interop & Docker Orchestration.        |
| 1.2     | 2026-04-17 | Nitish Singh | Integrated OpenTelemetry & Trace Context Propagation. |
| 1.3     | 2026-04-19 | Nitish Singh | M2 Hardware Saturation (4 P-Cores) & Batch Logic.     |

---

## Table of Contents

* [Overview](#overview)
* [Goals & Non-Goals](#goals)
  * [Goals](#goals)
  * [Non-Goals](#non-goals)
* [High-Level Architecture](#high-level-architecture)
* [Component Design](#component-design)
  * [C++ Conversion Engine (Native Layer)](#c-conversion-engine-native-layer)
  * [.NET Interop and Service Layer](#net-interop-and-service-layer)
  * [.NET Interop Layer](#net-interop-layer)
  * [REST API Layer](#rest-api-layer)
  * [Frontend UI](#frontend-ui)
* [Hardware Optimization (Apple M2)](#m2-hardware-optimization-v13)
* [Build & Deployment Architecture](#build--deployment-architecture)
* [Runtime Flow](#7-runtime-flow)
* [Error Handling](#8-error-handling)
* [Performance Considerations](#9-performance-considerations)
* [Extensibility](#10-extensibility)
* [Testing Strategy](#11-testing-strategy)
* [Deployment](#12-deployment)
* [Future Improvements](#13-future-improvements)
* [Tech Stack](#14-tech-stack)
* [Key Design Decisions](#15-key-design-decisions)
* [Tradeoffs](#16-tradeoffs)
* [Observability & Telemetry Architecture](#17-observability--telemetry-architecture)
* [Security & Boundary Hardening](#18-security--boundary-hardening)
* [Architectural Evolution](#architectural-evolution)

---

## Overview

The Case Conversion API is a multi-layered system that exposes a high-performance C++ string conversion engine through a .NET REST API and serves it to a React-based frontend UI. The architecture separates performance-critical logic from API orchestration and UI concerns.

The system uses native C++ for efficient string transformations, .NET for service exposure, and Docker for containerized deployment.

---

## Goals

* Performance: Utilize native C++ for O(n) string transformations.
* Scalability: Saturate M2 Performance Cores via parallel orchestration.
* Observability: Provide full*stack trace propagation across the ABI boundary.
* Security: Enforce a strict 5MB security gate for native memory protection.

---

## Non-Goals

* Persistent storage
* Authentication/authorization
* Distributed scaling
* Streaming or batch processing

---

## High-Level Architecture

```sh
        Frontend (React)
        |
        v
        ASP.NET Core REST API
        |
        v
        P/Invoke Interop Layer
        |
        v
        C++ Shared Library
        |
        v
        Strategy Pattern Conversion Engine
```

---

## Component Design

### C++ Conversion Engine (Native Layer)

Responsibilities:

* Implement conversion strategies
* Dispatch based on user choice
* Return processed string

Design Patterns:

* Strategy Pattern: Each conversion (SnakeCase, LeetSpeak, etc.) is an isolated strategy class.
* Factory Pattern: A central factory dispatches the input string to the correct strategy based on an integer `choice`.
* Memory Ownership: Follows the **"Callee-Allocates, Caller-Frees"** contract. The native engine allocates the result on the heap, and .NET is responsible for calling a `freeString` delegate.

Example Strategies:

* AlternatingCase
* CapitalizeWords
* SnakeCase
* KebabCase
* LeetSpeak
* Reverse
* ToggleCase

Factory selects strategy:
Input Choice → Factory → Strategy → Execute

### .NET Interop and Service Layer

The engine is exposed via a C-compatible interface.

Exported Function:

```C++
extern "C" const char* processStringDLL(const char* input, int choice);
```

Responsibilities:

* Bridge C++ and C#
* Dispatch conversion request
* Return C-style string pointer

#### Memory Ownership Policy

The Contract: To prevent memory leaks, the system follows a Caller-Must-Cleanup or Static-Return pattern.

Implementation: The C++ engine returns a const char*. The .NET side treats this as an IntPtr.

Safety: Because string conversions are stateless, we avoid long-lived heap allocations in the native layer to minimize the risk of fragmentation.

---

### .NET Interop Layer

Uses P/Invoke to call native DLL.

```C++
[DllImport("ProcessString")]
private static extern IntPtr processStringDLL(string input, int choice);
```

Responsibilities:

* Marshal strings
* Convert IntPtr to managed string
* Handle API-level validation

---

### REST API Layer

Endpoint:
POST /api/WordCase/convert

Request:
{
"input": "hello world",
"choice": 1
}

Response:
{
"output": "hElLo WoRlD"
}

Responsibilities:

* Input validation
* Call service layer
* Return JSON response

### M2 Hardware Optimization (v1.3)

To maximize throughput on Apple Silicon:

* P-Core Saturation: The system targets the 4 Performance Cores (P-Cores) of the M2.
* Parallel Orchestration: The `convert-batch` endpoint uses `Parallel.ForEachAsync` with a `MaxDegreeOfParallelism` of 4.
* N+1 Queueing: A `SemaphoreSlim` ensures that a 5th concurrent request is queued correctly, preventing the system from offloading high-priority work to the slower E-Cores (Efficiency Cores).

---

### Frontend UI

Technology:

* React
* TypeScript
* Vite

Responsibilities:

* Collect input
* Provide conversion selection
* Call REST API
* Display results

---

## Build & Deployment Architecture

### Multi-stage Docker Build

Stage 1:

* Build C++ shared library
* Publish .NET API

Stage 2:

* Copy artifacts
* Run ASP.NET runtime

---

## 7. Runtime Flow

```sh
            User Input (React)
            ↓
            REST API Call
            ↓
            ASP.NET Controller
            ↓
            ProcessStringService
            ↓
            P/Invoke
            ↓
            processStringDLL (C++)
            ↓
            Factory
            ↓
            Strategy
            ↓
            Result
            ↓
            API Response
            ↓
            Frontend Display
```

---

## 8. Error Handling

* Invalid choice → return lower case of string
* Null input → return empty string
* DLL load failure → HTTP 500
* API validation failure → HTTP 400

---

## 9. Performance Considerations

* Native C++ for string processing
* No heap allocations in critical path
* Static buffer reuse
* Minimal interop overhead

---

## 10. Extensibility

To add new conversion:

1. Create new Strategy class
2. Implement convert()
3. Register in Factory
4. Update enum mapping

No API changes required.

---

## 11. Testing Strategy

C++:

* GoogleTest unit tests
* Strategy validation

.NET:

* Service layer tests

Integration:

* API endpoint tests

---

## 12. Deployment

Backend:
Docker container on port 8080

Frontend:
Docker container on port 5173

Docker Compose orchestrates:

* frontend
* backend

---

## 13. Future Improvements

* SIMD optimizations
* Async API calls
* Plugin-based strategies
* Benchmark endpoints
* WASM build of C++ engine

---

## 14. Tech Stack

```sh
C++ — Conversion engine
CMake — Build system
GoogleTest — Unit tests
.NET 8 — REST API
React + TypeScript — UI
Docker — Containerization
GitHub Actions — CI/CD
```

---

## 15. Key Design Decisions

Decision: Use C++ for conversions
Reason: Performance and native interop demonstration

Decision: Use P/Invoke
Reason: Lightweight interop vs gRPC/native hosting

Decision: Strategy Pattern
Reason: Extensible conversion types

Decision: Multi-stage Docker
Reason: Smaller runtime image

Decision: Manual Telemetry Propagation
Reason: Ensures that errors occurring deep in the C++ engine are correlated to the specific HTTP request that triggered them.

---

## 16. Tradeoffs

Pros:

* High performance
* Modular design
* Extensible architecture

Cons:

* Native interop complexity
* Memory management risks
* Cross-platform build complexity

---

## 17. Observability & Telemetry Architecture

To ensure the polyglot boundary is transparent, the system implements:

* Trace Context Propagation: The .NET Gateway extracts the W3C traceparent and passes it to the native engine.

* Granular Spans: We track ABI Latency (the time spent converting data between managed and unmanaged memory) separately from Logic Latency (the time the C++ engine takes to run the strategy).

* Visualizer: All traces are exported via OTLP to a Jaeger/Zipkin backend.

## 18. Security & Boundary Hardening

* Buffer Overflow Protection: The managed API validates the input length against a MAX_BUFFER_SIZE before the P/Invoke call.

* Sanitization: The C++ engine uses std::string_view or bound-checked iterators to ensure it never reads past the memory allocated by the managed environment.

* Managed Handling**: The .NET layer detects this sentinel and throws a controlled `ArgumentException`, returning a `400 Bad Request` to the user instead of crashing the native process.

## Architectural Evolution

### v1.x: The Foundation (Single Instance)

* Architecture: Monolithic REST API wrapper over a Native Engine.

* Focus: Perfecting the ABI boundary and manual memory management.

* Limit: P(95) latency spikes during Garbage Collection (GC) events under high load.

### v2.0.0: Generation 2 (HA Cluster)

* Architecture: Layer 7 Load-Balanced Micro-Cluster.

* Mechanism: NGINX Reverse Proxy distributing traffic across 4 hardware-optimized replicas.

* Justification: Necessary to survive the 1,000,000 request endurance test.

* Result: Stabilized tail latency and 100% success rate by offloading pressure from single-process GC cycles.
