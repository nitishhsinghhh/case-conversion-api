# Case Conversion UI: React + TypeScript + Vite

This is a type-safe, modern frontend designed to interface with the high-performance Case Conversion API. It utilizes Vite for near-instant hot module replacement (HMR) and TypeScript for compile-time API contract validation.

## Architectural Features

- Service-Oriented Design: API calls are encapsulated in a dedicated service layer (wordCaseApi.ts).

- Type Safety: Request and Response models are shared conceptually with the backend DTOs to ensure data integrity.

- Asynchronous State Management: Utilizes modern async/await patterns for non-blocking UI updates during native code execution.

## Manual Local Setup

Prerequisites: Node.js (v18+) and npm.

```Bash
# Navigate to the UI directory:

cd string-conversion-ui
Install dependencies:

# Install dependencies:
npm install
Run Development Server:

# Run Development Server:
npm run dev
``

The UI will be accessible at http://localhost:5173.


## Automated Project Initialization

A shell script is provided to scaffold the architectural structure if you are resetting the environment:

```Bash
chmod +x setup-ui.sh
./setup-ui.sh
```

## Getting Started: The Two-Path Approach

### Path A: The Local Development Path (Manual)

Best for debugging specific layers of the stack.

```Bash
# 1. Build the Native Engine:

cd CaseConversionAPI/CppLib && mkdir -p build && cd build
cmake .. && cmake --build . --config Release
Start the .NET Gateway:

# 2. Start the .NET Gateway:
cd ../../DotNetAPI
dotnet run
Launch the UI:

# 3. Launch the UI:
cd ../../string-conversion-ui
npm install && npm run dev
```

### Path B: The Principal's Path (Docker Compose)

Best for verifying the full system in a production-like environment.

```Bash
# Build and orchestrate all services
docker compose up --build
```

## Security & Reliability Summary

| Layer          | Responsibility       | Security / Safety Mechanism |
|----------------|----------------------|------------------------------|
| Frontend       | User Interaction     | TypeScript interfaces & input sanitization |
| API Gateway    | Orchestration        | `IDisposable` and `NativeLibrary` handle management |
| Native ABI     | High-Perf Logic      | 2MB buffer guard & explicit `freeString` contract |
| Infrastructure | Environment Parity   | Multi-stage Docker with artifact promotion |
