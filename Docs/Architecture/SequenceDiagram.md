# Request Processing Sequence

## Convert Request Flow

```text
Client
 │
 │ POST /api/WordCase/convert
 ▼
WordCaseController
 │
 ▼
ProcessService
 │
 ▼
NativeInterop
 │
 ▼
processStringDLL()
 │
 ▼
ConversionEngine
 │
 ▼
StrategyFactory
 │
 ▼
Selected Strategy
 │
 ▼
Result
 │
 ▼
ASP.NET Response
 │
 ▼
Client
```

## Execution Summary

1. Client submits conversion request.
2. Controller validates payload.
3. Service coordinates execution.
4. Native interop invokes exported DLL function.
5. Strategy is selected based on conversion type.
6. Processing result is returned.
7. Response is serialized and returned to the client.
