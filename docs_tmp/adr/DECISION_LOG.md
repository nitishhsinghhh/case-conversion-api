# Architectural Decision Log: Polyglot String Engine

> **"The important thing about a problem is not its solution, but the strength we gain in finding the solution."**  
> This log serves as the authoritative record for the architectural evolution of the Case Conversion Engine. It documents the trade-offs, constraints, and strategic reasoning behind the system's current state.

---

## Table of Contents

1. [Systems Architecture & Interop (ADR 001-009)](#category-1-systems-architecture-and-interop)
2. [Performance & Hardware Optimization (ADR 010-015)](#category-2-performance-and-hardware)
3. [Security & Error Handling (ADR 016-021)](#category-3-security-and-error-handling)
4. [Observability & DevOps (ADR 022-027)](#category-4-observability-and-devops)
5. [Development Experience (DevEx) (ADR 028-030)](#category-5-development-experience)

---

## Category 1: Systems Architecture and Interop

| ID | Decision | Context / Rationale |
| :--- | :--- | :--- |
| **001** | **P/Invoke vs gRPC** | Chosen for sub-millisecond in-process latency; network RPC overhead was deemed unacceptable for high-frequency string logic. |
| **002** | **Callee-Allocates** | Ensures the C++ allocator owns the buffer creation, while C# handles lifecycle disposal via an exported `free` delegate. |
| **004** | **Strategy Pattern** | Encapsulated conversion logic into discrete classes to allow runtime algorithm switching without modifying the core dispatcher. |
| **005** | **Static Factory** | Decoupled client code from concrete strategy implementations, simplifying the integration of new conversion types. |
| **006** | **NativeLibrary.TryLoad** | Enabled dynamic runtime loading of shared objects (`.dylib`/`.so`), providing platform-agnostic flexibility without static linking. |
| **007** | **extern "C" Exports** | Applied to bypass C++ name mangling, ensuring a stable ABI for the .NET runtime to locate function entry points. |
| **008** | **Manual P/Invoke** | Rejected auto-wrappers (SWIG) in favor of manual signatures to maintain 100% control over memory pinning and marshalling. |
| **009** | **UTF-8 (char*) Standard** | Standardized on UTF-8 across the boundary to match .NET's internal string representation and avoid expensive transcoding. |

### ADR 001: Selection of P/Invoke over gRPC

- Context: Need a high-frequency bridge between .NET and C++.
- Alternatives Considered: gRPC (Sidecar), Unix Domain Sockets.
- Decision: In-process P/Invoke.
- Pros: Zero network stack overhead; sub-microsecond call latency.
- Cons: Tight coupling; a crash in native code kills the managed process.

### ADR 002: Callee-Allocates Memory Contract

- Context: Managing ownership of strings returned from C++.
- Decision: C++ allocates the buffer; C# calls an exported free function.
- Pros: Prevents CRT mismatch errors; explicit ownership prevents leaks.
- Cons: Requires rigorous use of try/finally in C# to ensure deallocation.

### ADR 006: Dynamic Library Discovery via NativeLibrary.TryLoad

- Context: Supporting .dylib (macOS) and .so (Linux) without hardcoding paths.
- Decision: Use the .NET NativeLibrary class for dynamic loading.
- Pros: Platform-agnostic; allows for runtime "graceful degradation" if a library is missing.
- Cons: Slightly more boilerplate than [DllImport].

---

## Category 2: Performance and Hardware

| ID | Decision | Context / Rationale |
| :--- | :--- | :--- |
| **003** | **M2 P-Core Affinity** | Capped concurrency at 4 to align with Apple M2 Performance Cores, avoiding the "E-Core management tax." |
| **010** | **ConcurrentBag<T>** | Used for thread-safe native data collection during batch runs, minimizing lock-contention overhead. |
| **011** | **K6 over JMeter** | Selected for native Go-based ARM64 execution, ensuring the load generator didn't bottleneck the M2 SoC. |
| **012** | **Multi-stage Docker** | Utilized separate Build and Runtime stages to strip toolchains, reducing image size by ~70%. |
| **013** | **GC Latency Tuning** | Set `GCLatencyMode` to `SustainedLowLatency` to prevent "Stop-the-World" pauses during 1M request stress tests. |
| **014** | **No Memory Mapping** | Rejected Memory-Mapped Files for buffers <5MB; the overhead of file I/O exceeded the cost of heap allocation. |
| **015** | **Alpine Linux (NGINX)** | Optimized the ingress container for size and security by using a minimal Alpine footprint. |

---

## Category 3: Security and Error Handling

| ID | Decision | Context / Rationale |
| :--- | :--- | :--- |
| **016** | **Sentinel Pattern** | Native errors return specific overflow/error codes, which are caught and re-thrown as managed Exceptions in C#. |
| **017** | **5MB Buffer Ceiling** | Implemented a hard limit on input strings to mitigate Heap Spraying and buffer exhaustion attacks. |
| **018** | **Apache-2.0 License** | Selected to provide a balance of open-source freedom and corporate patent protection. |
| **019** | **Non-Root Docker** | Configured containers to run as `nonroot` users to prevent privilege escalation in the event of a breach. |
| **020** | **NGINX Rate Limiting** | Throttled ingress traffic to 3.5k req/s to match the verified hardware ceiling and prevent DDoS-induced thermal throttling. |
| **021** | **No Native Exceptions** | Enforced a strict "No Throw" policy across the ABI; exceptions are caught in C++ and returned as status codes. |

---

## Category 4: Observability and DevOps

| ID | Decision | Context / Rationale |
| :--- | :--- | :--- |
| **022** | **OpenTelemetry (OTLP)** | Standardized on OTLP to remain vendor-neutral, allowing for easy switching between Jaeger, Honeycomb, or Datadog. |
| **023** | **W3C TraceContext** | Propagated `traceparent` headers into native logs to correlate C++ debug info with specific HTTP requests. |
| **024** | **GitHub Actions** | Integrated CI/CD natively into the VCS to handle multi-platform builds for ARM64 and x64. |
| **025** | **Environment Promotion** | Enforced immutable artifact promotion (Dev → Staging → Prod) to ensure environmental parity. |
| **026** | **Docker Healthchecks** | Configured NGINX upstreams to only route traffic to containers passing the `.NET HealthCheck` endpoint. |
| **027** | **Structured Logging** | Native engine emits logs in JSON format, facilitating easy ingestion into ELK/Splunk stacks. |

---

## Category 5: Development Experience

| ID | Decision | Context / Rationale |
| :--- | :--- | :--- |
| **028** | **CMake Build System** | Used to manage the C++ toolchain across Windows (MSVC) and macOS (Clang) without duplicate project files. |
| **029** | **.devcontainer** | Provided a pre-configured VS Code environment to reduce "Time-to-Hello-World" for new contributors to zero. |
| **030** | **GoogleTest (GTest)** | Selected for native-layer unit testing due to its robust support for C++17 features and mock objects. |

---

## Highlight: ADR 013 (Sustained Low Latency)

- **Problem:** Tail latency spikes (>10s) during the 1,000,000 request marathon.

- **Root Cause:** Standard GC behavior was performing full collection cycles under heavy P/Invoke pressure.

- **The Solution:** By switching to `SustainedLowLatency`, we prioritized the **95th percentile (P95)** over absolute memory efficiency. This resulted in a rock-solid 58.8ms P95, proving that at a certain scale, hardware-aware tuning is a mechanical necessity.

---

[↑ Back to Top](#architectural-decision-log-polyglot-string-engine)
