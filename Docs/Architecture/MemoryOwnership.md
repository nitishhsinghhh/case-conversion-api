# Native-to-Managed Memory Ownership Model

## Objective

Prevent memory leaks, allocator mismatch, and double-free conditions across the .NET and C++ boundary.

## Ownership Flow

```text
.NET Runtime
     │
     │ P/Invoke
     ▼
C++ Engine
     │
     │ Allocates char*
     ▼
Returned Buffer
     │
     ▼
.NET Marshals String
     │
     │ Calls freeMemoryDLL()
     ▼
C++ Releases Buffer
```

## Ownership Rules

### Native Runtime

Responsible for:

* Buffer allocation
* Internal object lifetime
* RAII cleanup

### Managed Runtime

Responsible for:

* Marshaling results
* Invoking cleanup APIs
* Releasing unmanaged resources

## Safety Guarantees

* Deterministic ownership transfer
* No allocator mismatch
* No shared mutable state
* Predictable cleanup under failure conditions

## Exported ABI

```cpp
extern "C"
{
    char* processStringDLL(...);

    void freeMemoryDLL(char*);
}
```

This contract defines the ownership boundary between managed and unmanaged execution environments.
