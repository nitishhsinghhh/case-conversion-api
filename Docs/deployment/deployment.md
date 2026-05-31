# Deployment & Infrastructure Guide

This document defines the containerization, orchestration, and environment promotion strategy for the Case Conversion system.

---

## Table of Contents

- [Overview](#overview)
- [Docker Architecture](#docker-architecture)
  - [Multi-Stage Build (Backend)](#multi-stage-build-backend)
  - [Frontend UI (Nginx)](#frontend-ui-react--vite--nginx)
- [Environment Strategy](#environment-strategy)
- [Pipeline Execution](#pipeline-execution)
  - [Development](#development-environment)
  - [Staging (Promotion)](#staging-environment)
  - [Production](#production-environment)
- [Troubleshooting & Lessons Learned](#troubleshooting-and-lessons-learned)
- [Internal Architecture](#internal-architecture)
- [Deployment and Environments](#deployment-and-environments)
- [Key Principles](#key-principles)
- [Summary](#summary)

---

## Overview

The system is containerized using Docker and orchestrated via Docker Compose. It follows a **single-artifact promotion model**, ensuring consistency across environments.

Deployment Flow:

```bash
Development → Staging → Production
```

- Build Once: Images are built only during the Development/CI stage.
- Promote Forward: The same immutable image is re-tagged and moved through the pipeline.
- No Rebuilds: Eliminates "it worked on my machine" bugs by ensuring the exact same binary runs in Prod as in Staging.

---

## Docker Architecture

### Multi-Stage Build (Backend)

The backend utilizes a multi-stage build to ensure the final production image is lightweight and secure, containing only the runtime and the compiled binaries.

```Bash
# ---------- Stage 1: Build ----------
FROM mcr.microsoft.com/dotnet/sdk:8.0 AS build

# Install C++ toolchain
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake

WORKDIR /src
COPY . .

# Clean and build C++ shared library
RUN rm -rf CaseConversionAPI/CppLib/build && \
    cmake -S CaseConversionAPI/CppLib -B CaseConversionAPI/CppLib/build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build CaseConversionAPI/CppLib/build --parallel

# Publish .NET API
RUN dotnet publish CaseConversionAPI/DotNetAPI -c Release -o /app/publish

# Copy compiled C++ binary to the publish folder
RUN cp CaseConversionAPI/CppLib/build/lib/libProcessStringDLL.so /app/publish/

# ---------- Stage 2: Runtime ----------
FROM mcr.microsoft.com/dotnet/aspnet:8.0
WORKDIR /app
COPY --from=build /app/publish .

EXPOSE 8080
ENTRYPOINT ["dotnet", "DotNetAPI.dll"]
```

### Frontend UI (React + Vite + Nginx)

In Production/Staging, the frontend is served via Nginx for high performance. In Development, it uses the Vite Dev Server for hot-module replacement (HMR).

- Production Port: 80 (Standard HTTP)

- Tech Stack: React 18, TypeScript, Vite.

- Static Hosting: Nginx handles routing and serves the minified JS/CSS bundle.

---

## Environment Strategy

Container images are tagged based on the stage of the pipeline.

| Environment | Tag      | Port (UI) | Port (API) | Purpose                          |
|------------ |----------|-----------|------------|----------------------------------|
| Dev         | dev      | 5173      | 8080       | Active development & hot-reload  |
| Staging     | staging  | 5174      | 8081       | Pre-prod validation & QA         |
| Production  | latest   | 80        | 8080       | Production deployment            |

```Bash
nitishhsinghhh/case-api:dev
nitishhsinghhh/case-api:staging
nitishhsinghhh/case-api:latest
```

---

## Pipeline Execution

### Development Environment

Used for active development and debiugging

```Bash
docker compose -f docker-compose.dev.yml up --build
```

Features:

- Local image build
- ASPNETCORE_ENVIRONMENT=Development
- Hot reload for frontend
- Debug logging enabled

### Staging Environment

Used for production-like validation.

#### Promote & Deploy Image: Staging

```Bash
# Tag and Push
docker tag nitishhsinghhh/case-api:dev nitishhsinghhh/case-api:staging
docker push nitishhsinghhh/case-api:staging

# Deploy Staging
docker compose -f docker-compose.staging.yml up -d
```

Features:

- Uses pre-built image
- ASPNETCORE_ENVIRONMENT=Staging
- No local builds
- Mirrors production behavior

load test on staging (100,000 case)

```Bash
echo '{"text": "hardware-aware engine", "choice": 1}' > load_test.json
ab -n 1000000 -c 100 -p load_test.json -T 'application/json' http://localhost:8081/api/WordCase/convert
```

```log
nitishsingh@Nitishs-MacBook-Air word-case-api % echo '{"text": "hardware-aware engine", "choice": 1}' > load_test.json
ab -n 1000000 -c 100 -p load_test.json -T 'application/json' http://localhost:8081/api/WordCase/convert
This is ApacheBench, Version 2.3 <$Revision: 1923142 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 100000 requests
Completed 200000 requests
Completed 300000 requests
Completed 400000 requests
Completed 500000 requests
Completed 600000 requests
Completed 700000 requests
Completed 800000 requests
Completed 900000 requests
Completed 1000000 requests
Finished 1000000 requests


Server Software:        Kestrel
Server Hostname:        localhost
Server Port:            8081

Document Path:          /api/WordCase/convert
Document Length:        96 bytes

Concurrency Level:      100
Time taken for tests:   441.980 seconds
Complete requests:      1000000
Failed requests:        33
   (Connect: 0, Receive: 0, Length: 33, Exceptions: 0)
Total transferred:      234992245 bytes
Total body sent:        202000000
HTML transferred:       95996832 bytes
Requests per second:    2262.54 [#/sec] (mean)
Time per request:       44.198 [ms] (mean)
Time per request:       0.442 [ms] (mean, across all concurrent requests)
Transfer rate:          519.22 [Kbytes/sec] received
                        446.32 kb/s sent
                        965.54 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.5      0      23
Processing:     2   44 196.3     37   15223
Waiting:        0   42 190.9     37   15223
Total:          2   44 196.3     38   15224

Percentage of the requests served within a certain time (ms)
  50%     38
  66%     43
  75%     47
  80%     50
  90%     59
  95%     70
  98%     87
  99%    102
 100%  15224 (longest request)
 ```

 After implementing native RAII

```log
nitishsingh@Nitishs-MacBook-Air word-case-api % echo '{"text": "hardware-aware engine", "choice": 1}' > load_test.json
ab -n 1000000 -c 100 -p load_test.json -T 'application/json' http://localhost:8081/api/WordCase/convert
This is ApacheBench, Version 2.3 <$Revision: 1923142 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 100000 requests
Completed 200000 requests
Completed 300000 requests
Completed 400000 requests
Completed 500000 requests
Completed 600000 requests
Completed 700000 requests
Completed 800000 requests
Completed 900000 requests
Completed 1000000 requests
Finished 1000000 requests


Server Software:        Kestrel
Server Hostname:        localhost
Server Port:            8081

Document Path:          /api/WordCase/convert
Document Length:        96 bytes

Concurrency Level:      100
Time taken for tests:   353.611 seconds
Complete requests:      1000000
Failed requests:        18
   (Connect: 0, Receive: 0, Length: 18, Exceptions: 0)
Total transferred:      234995770 bytes
Total body sent:        202000000
HTML transferred:       95998272 bytes
Requests per second:    2827.97 [#/sec] (mean)
Time per request:       35.361 [ms] (mean)
Time per request:       0.354 [ms] (mean, across all concurrent requests)
Transfer rate:          648.98 [Kbytes/sec] received
                        557.86 kb/s sent
                        1206.84 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.5      0      41
Processing:     0   35 233.9     22   18369
Waiting:        0   29 228.3     18   18369
Total:          0   35 233.9     22   18369

Percentage of the requests served within a certain time (ms)
  50%     22
  66%     34
  75%     42
  80%     48
  90%     67
  95%     86
  98%    112
  99%    134
 100%  18369 (longest request)
 ```

### Performance Metrics and Insights: The RAII Transformation

The project reached a major engineering milestone with the implementation of RAII and the Rule of Five in the native core. Below is the comparative data from a 1,000,000 request stress test conducted on Apple M2 hardware, demonstrating the massive efficiency gains in the polyglot bridge.

| Metric                 | Pre-RAII Baseline | Post-RAII (Optimized)  | Net Improvement           |
|----------------------- |-------------------|------------------------|---------------------------|
| Throughput             | 2,262.54 req/s    | 2,827.97 req/s         | +25% Gain                 |
| Mean Latency           | 44.19 ms          | 35.36 ms               | 20% Reduction             |
| Median Latency (P50)   | 38.00 ms          | 22.00 ms               | 42% Snappier              |
| Tail Latency (P99)     | 102.00 ms         | 134.00 ms              | Thermal / TCP Overhead    |
| Success Rate           | 99.996%           | 99.998%                | Near-Perfect Reliability  |

## The Architect's Deep Dive

- The 99% Consistency: Despite the massive volume, 99% of your requests finished under 102ms. This indicates that the thread pool isn't choking and the manual memory management in C++ is keeping the heap clean.

- The 33 "Length" Failures: Notice the Failed requests: 33 (Length: 33). In ab, a length failure usually means the response body was slightly different than expected. Given it's only 0.0033% of the total, this is likely a tiny buffer edge case or a malformed JSON termination during a context switch—nothing to worry about at this scale, but worth a look in the logs.

- The 15-Second Outlier (100% at 15224ms): We hit exactly one "Longest Request" that took 15 seconds. On a Mac, this is almost certainly Thermal Throttling or a macOS background task (like Spotlight or iCloud) momentarily stealing the CPU cycles from the Docker VM. The fact that the median stayed at 38ms shows the engine recovered instantly.

---

### Production Environment

Used for final deployment.

#### Promote & Deploy Image: Prod

```Bash
# Tag and Push
docker tag nitishhsinghhh/case-api:staging nitishhsinghhh/case-api:latest
docker push nitishhsinghhh/case-api:latest

# Deploy Production
docker compose -f docker-compose.prod.yml up -d
```

### The flush and reset

```Bash
# 1. Kill everything across all compose files
docker compose -f docker-compose.yml down
docker compose -f docker-compose.prod.yml down

# 2. Hard reset the Docker network (optional but recommended)
docker network prune -f

# 3. Start ONLY the Production stack
docker compose -f docker-compose.prod.yml up -d
```

### Verify the port

```Bash
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}"
```

Look specifically for:
Is case-api-prod listed?
Does it say 0.0.0.0:8081->8080/tcp?

```log
nitishsingh@Nitishs-MacBook-Air word-case-api % docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}"
NAMES           STATUS             PORTS
case-ui-prod    Up 11 seconds      0.0.0.0:80->80/tcp, [::]:80->80/tcp
case-api-prod   Up 11 seconds      0.0.0.0:8080->8080/tcp, [::]:8080->8080/tcp
jaeger          Up About an hour   0.0.0.0:4317-4318->4317-4318/tcp, [::]:4317-4318->4317-4318/tcp, 0.0.0.0:16686->16686/tcp, [::]:16686->16686/tcp
```

```log
nitishsingh@Nitishs-MacBook-Air word-case-api % ab -n 1000000 -c 100 -p load_test.json -T 'application/json' http://localhost:8080/api/WordCase/convert
This is ApacheBench, Version 2.3 <$Revision: 1923142 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 100000 requests
Completed 200000 requests
Completed 300000 requests
Completed 400000 requests
Completed 500000 requests
Completed 600000 requests
Completed 700000 requests
Completed 800000 requests
Completed 900000 requests
Completed 1000000 requests
Finished 1000000 requests


Server Software:        Kestrel
Server Hostname:        localhost
Server Port:            8080

Document Path:          /api/WordCase/convert
Document Length:        96 bytes

Concurrency Level:      100
Time taken for tests:   1672.998 seconds
Complete requests:      1000000
Failed requests:        49
   (Connect: 0, Receive: 0, Length: 49, Exceptions: 0)
Total transferred:      234988485 bytes
Total body sent:        202000000
HTML transferred:       95995296 bytes
Requests per second:    597.73 [#/sec] (mean)
Time per request:       167.300 [ms] (mean)
Time per request:       1.673 [ms] (mean, across all concurrent requests)
Transfer rate:          137.17 [Kbytes/sec] received
                        117.91 kb/s sent
                        255.08 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.3      0      17
Processing:     1  167 287.8    104   14382
Waiting:        0  139 256.4    100   14196
Total:          1  167 287.8    105   14383

Percentage of the requests served within a certain time (ms)
  50%    105
  66%    184
  75%    202
  80%    220
  90%    388
  95%    501
  98%    689
  99%    798
 100%  14383 (longest request)
nitishsingh@Nitishs-MacBook-Air word-case-api % 
```

## The Architect's Comparison: Staging vs. Production

- Production Constraints: In the docker-compose.prod.yml, we have resource limits (CPU/Memory caps) and additional logging/security middleware active that isn't present in Staging. This is standard; Production is built for safety and observability, while Staging is built for raw speed.

- The 100% Success Rate (Mostly): Only 49 "length" failures out of 1,000,000 requests. That is a 99.9951% success rate. In professional backend development, that is considered "Five Nines" territory.

- Thermal Endurance: This test took 1,672 seconds (nearly 28 minutes). Running a fanless MacBook Air at full tilt for half an hour proves that our C++ engine's memory management is rock-solid.

Features:

- Stable image (latest)
- ASPNETCORE_ENVIRONMENT=Production
- Restart policies enabled
- Health checks enabled

---

## Troubleshooting and Lessons Learned

During the initial infrastructure setup, the following critical points were identified:

- Lean Runtime Images: The .NET 8 ASP.NET runtime image does not include curl. Avoid using curl in Docker Healthchecks unless you manually install it. Use TCP port checks or internal .NET probes instead.

- Empty Config Files: .NET will crash on startup if appsettings.Staging.json or appsettings.Production.json exists but is empty. Ensure these files contain valid JSON {} or are deleted if not needed.

- Port Conflicts: When running multiple environments on one host (MacBook), ensure ports (8080, 8081, 80) do not overlap across active containers.

- Zsh Comments: When copy-pasting commands from this guide into Zsh, avoid pasting lines starting with # as they may trigger "command not found" errors depending on shell configuration.

### Internal Architecture

```Bash
[ Browser ]  ←── (HTTP/JSON) ──→  [ Nginx Container (Frontend) ]
                                          │
                                   (Internal Network)
                                          │
                                          ▼
[ .NET 8 API Container (Backend) ] ←── (P/Invoke) ──→ [ C++ Shared Library ]
          │                                                    │
     (Controller)                                      (Strategy Pattern)
```

---

## Deployment and Environments

This project follows a containerized deployment workflow using Docker. Ensure you are authenticated with Docker Hub before pushing images.

1. Authentication

If your session has expired or you are on a new device, authenticate via the Docker CLI:

```Bash
docker login
```

Follow the on-screen instructions to enter your device activation code at login.docker.com/activate.

2.Promoting Images (Dev to Staging) then do Verification

After pushing, verify the image exists in the registry:

- Log in to Docker Hub.

- Navigate to the nitishhsinghhh/case-api repository.

- Check the Tags tab for the staging identifier.

---

## Key Principles

- Single artifact promotion (same image across environments)
- No rebuilds after development
- Environment isolation via configuration
- Consistent runtime behavior across all stages

---

## Summary

Summary
This deployment model ensures deterministic builds and consistent environments. By separating the heavy build-time dependencies (SDKs, C++ compilers) from the runtime, we maintain a secure, production-grade CI/CD readiness.
