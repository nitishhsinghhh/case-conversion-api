# High-Performance Polyglot Engine: Performance Benchmarks

## Overview

This documents the performance of a high-efficiency string transformation engine. The system utilizes a Native C++ Core integrated into a C# (.NET) REST API. Benchmarking was performed on Apple Silicon (M2) to measure the limits of throughput, latency, and hardware saturation in a polyglot architecture.

## Hardware Specifications

- Processor: Apple M2 (8-core: 4 Performance, 4 Efficiency)

- Architecture: ARM64

- Memory: Unified Memory Architecture

- Environment: Fanless (MacBook Air)

## Benchmark Results: The Scaling Journey

We progressed through multiple testing phases to map the performance curve, ranging from single-request baselines to extreme stress tests.

| Iterations   | VUs (Concurrency) | RPS (Throughput) | p95 Latency | Status                                      |
|--------------|------------------ |------------------|-------------|---------------------------------------------|
| 10 - 100     | 1                 | ~1,200           | < 1ms       | Cold Start: Initializing JIT & Memory       |
| 1,000        | 4                 | 4,526            | 1.82ms      | Baseline: Perfect core alignment            |
| 100,000      | 8                 | 7,242            | 2.66ms      | Peak Efficiency: Full hardware saturation   |
| 1,000,000    | 100               | 4,793            | 9.00ms      | Stress: High concurrency/contention         |
| 1,500,000    | 100               | 7,067            | 41.0ms      | Endurance: Long-duration stability          |

---

## Key Technical Findings

### 1. The Hardware "Sweet Spot" (8 VUs)

The system achieves maximum throughput at 8 Virtual Users. This correlates perfectly with the 8 physical cores of the M2 chip.

- Finding: Doubling concurrency from 4 to 8 VUs increased throughput by 44%.

- Constraint: Beyond 8 VUs, throughput plateaus or decreases due to context switching overhead, as the OS must swap 100 threads across only 8 physical cores.

### 2. Real-World User Capacity

Using a standard 5-second "Human Think Time" (the time between user clicks), this setup's peak throughput of 7,242 RPS translates to:

- ~36,200 Concurrent Human Users

- This demonstrates that a single optimized laptop can serve the traffic of a medium-to-large scale web application.

### 3. Latency & User Experience

- Elite Tier: The average response time of 1.13ms at peak load is approximately 40x faster than a human blink.

- Consistency: Under a 150k iteration load, 95% of users (p95) received a response in under 50ms, which feels "instant" in a web browser.

- Saturation Point: During the 1.5M iteration test, the Max Latency hit 13.4s. This indicates that while the system never failed, it reached a "queuing" state where requests were backed up.

### 4. Stability & Reliability

- Zero Failures: Across over 3 million total checks, the system maintained a 100% success rate.

- Thread Safety: The C++ core remained stable under 100-thread concurrency, proving the robustness of the C#/C++ memory boundary.

---

## How to Run

In the write file, provide these values 

```Bash
export const options = {
  vus: 100,
  iterations: 1500000, 
};
```

Then run the following comamnd from the terminal.

Run the stack in the background:

```Bash
docker compose -f docker-compose-load.yml up -d
```

```Bash
docker run --rm -i \
  --add-host=host.docker.internal:host-gateway \
  --add-host=loadbalancer:host-gateway \
  -v $(pwd):/results \
  grafana/k6 run --summary-export=/results/result.json - <writeFile.js
```
