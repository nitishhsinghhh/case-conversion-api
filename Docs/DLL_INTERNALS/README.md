# Windows DLL & Native Interop Internals

The Native Conversion Engine in this project is compiled as a Dynamic-Link Library (DLL). Unlike static libraries, which are merged at compile-time, our architecture leverages Dynamic Linking for several strategic reasons:

## 1. Memory Efficiency & Shared Pages

By using a DLL, the Windows OS can map the same physical memory pages of our libProcessStringDLL.dll into multiple application processes.

- Static Linking: Copies object code into every executable (Wasteful).

- Dynamic Linking: Dynamic linking allows multiple processes to map the same DLL image, enabling Windows to share eligible read-only/code pages between processes. Each process still receives its own virtual address mapping and process-specific writable state.

## 2. The Loading Lifecycle

The project supports both Implicit and Explicit linking:

- Implicit (Load-time): A native executable can declare DLL dependencies through its PE import table. The Windows loader resolves those dependencies during process initialization.

- Explicit (Run-time): The application can load the DLL at runtime using APIs such as LoadLibrary/GetProcAddress (or .NET's NativeLibrary.Load/function-resolution mechanisms). This gives the application control over when the library is loaded and which exported symbols are resolved.

## 3. Application vs. DLL Ownership

A key architectural constraint we respect is that a DLL does not own its own process space.

- A DLL loaded into a process executes in the host process's address space. Its functions execute on the calling thread and share the process's virtual memory, heap environment, handles, and process lifetime.

- Thread Safety: The native conversion path is designed to be reentrant: request-specific state is kept local to each invocation, while shared process-wide state is immutable or explicitly synchronized. This allows concurrent .NET ThreadPool calls to enter the native engine without request-level shared mutable state.

## 4. Language Agnosticism (Polyglot Bridge)

The DLL acts as a "Universal Translator." Because it follows the standard C Calling Convention (__cdecl), our C++17 logic can be consumed by:

- The .NET 8 Gateway (Current implementation).

- Future Python or Rust wrappers without rewriting the core conversion logic.
