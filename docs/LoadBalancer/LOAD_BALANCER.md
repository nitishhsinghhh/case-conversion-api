# Load Balancing & Orchestration (NGINX)

This document explains the Layer 7 load balancing strategy used to scale the Polyglot Case Conversion API.

## Table of Contents

- [What is a Load Balancer?](#1-what-is-a-load-balancer)
- [How it is Used in This Project](#2-how-it-is-used-in-this-project)
- [Configuration Breakdown](#3-configuration-breakdown)
- [Performance Impact (The 1M Test)](#4-performance-impact-the-1m-test)
- [Orchestration Summary](#5-summary-table)
  
---

## 1. What is a Load Balancer?

A Load Balancer acts as a "traffic cop" sitting in front of your servers. It routes incoming client requests across all servers capable of fulfilling those requests in a manner that maximizes speed and capacity utilization and ensures that no one server is overworked.

### Why use one?

- High Availability: If one container crashes, the balancer redirects traffic to the healthy ones.

- Scalability: You can add more backend "workers" without changing the URL the user sees.

- Performance: It distributes the heavy C++ string transformation tasks across multiple CPU cores.

---

## 2. How it is Used in This Project

In this system, we use NGINX as a Reverse Proxy and Load Balancer. It is the entry point for all API traffic.

The Architecture

1. Ingress: All requests hit NGINX on port 8080.

2. Algorithm: We use Round Robin distribution.

3. Backends: Traffic is forwarded to a pool of 4 .NET API replicas.

---

## 3. Configuration Breakdown

nginx.conf

The core of our balancing logic resides in the upstream block:

```Nginx
upstream backend_servers {
    server backend:8080; # Docker DNS resolves this to all running replicas
}

server {
    listen 80;
    location / {
        proxy_pass http://backend_servers;
    }
}
```

docker-compose-load.yml

We use Docker Compose to manage the life-cycle of these replicas:

```Yaml
services:
  backend:
    image: case-conversion-api-backend
    deploy:
      replicas: 4  # Matches the 4 P-Cores of the M2 chip
```

---

## 4. Performance Impact (The 1M Test)

During the 1,000,000 request stress test, the load balancer played a critical role in maintaining stability:

- Latency Smoothing: By spreading requests, we prevented any single .NET process from becoming overwhelmed by Garbage Collection (GC) pauses.

- Horizontal Scale: While a single instance might throttle at 800 req/s, our 4-replica balanced cluster sustained 2,511 req/s.

- Zero Failures: NGINX managed the connection pooling, ensuring that even under extreme 50-VU concurrency, no TCP connections were dropped.

---

## 5. Summary Table

| Feature        | Implementation                                  |
|----------------|-------------------------------------------------|
| Software       | NGINX (Alpine-based ARM64 image)                |
| Strategy       | Round Robin                                     |
| Max Replicas   | 4 (Hardware-optimized for M2)                   |
| Health Checks  | Automatic (NGINX failure detection)             |

---
