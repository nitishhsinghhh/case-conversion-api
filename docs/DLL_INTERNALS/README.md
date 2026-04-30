# Windows DLL & Native Interop Internals

The Native Conversion Engine in this project is compiled as a Dynamic-Link Library (DLL). Unlike static libraries, which are merged at compile-time, our architecture leverages Dynamic Linking for several strategic reasons:

## 1. Memory Efficiency & Shared Pages

By using a DLL, the Windows OS can map the same physical memory pages of our libProcessStringDLL.dll into multiple application processes.

- Static Linking: Copies object code into every executable (Wasteful).

- Dynamic Linking: One copy in memory, shared across the system (Optimized).

## 2. The Loading Lifecycle

The project supports both Implicit and Explicit linking:

- Implicit (Load-time): The Windows loader uses the generated Import Library (.lib) to resolve symbols as soon as the .NET Gateway starts.

- Explicit (Run-time): Allows for "Hot-Swapping" logic or loading the DLL only when a specific high-performance request hits the API.

## 3. Application vs. DLL Ownership

A key architectural constraint we respect is that a DLL does not own its own process space.

- The DLL operates within the stack and thread context of the .NET 8 host.

- Thread Safety: Because the DLL shares the host's memory, we implemented Stateless Execution to ensure that concurrent calls from the .NET ThreadPool do not cause race conditions.

## 4. Language Agnosticism (Polyglot Bridge)

The DLL acts as a "Universal Translator." Because it follows the standard C Calling Convention (__cdecl), our C++17 logic can be consumed by:

- The .NET 8 Gateway (Current implementation).

- Future Python or Rust wrappers without rewriting the core conversion logic.
