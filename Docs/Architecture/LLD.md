# Low Level Design (LLD)

## Core Class Relationships

```text
WordCaseController
        │
        ▼
ProcessService
        │
        ▼
NativeInterop
        │
        ▼
ConversionEngine
        │
        ▼
StrategyFactory
        │
        ├── UpperCaseStrategy
        ├── LowerCaseStrategy
        ├── TitleCaseStrategy
        └── SpellCheckStrategy
```

## Design Patterns

### Strategy Pattern

Encapsulates individual conversion algorithms behind a common interface.

```text
IConversionStrategy
        ▲
        │
   Concrete Strategies
```

### Factory Pattern

Responsible for selecting and creating the appropriate strategy implementation.

### Proxy Pattern

The .NET service acts as a managed proxy over the native C++ library.

## Layer Responsibilities

| Layer      | Responsibility           |
| ---------- | ------------------------ |
| Controller | HTTP handling            |
| Service    | Business orchestration   |
| Interop    | Native invocation        |
| Engine     | Processing execution     |
| Strategy   | Algorithm implementation |
