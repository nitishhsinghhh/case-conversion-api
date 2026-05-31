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

A Load Balancer sits in front of backend services and intelligently distributes requests across multiple servers to improve scalability, availability, and system stability under concurrent load.

### Why use one?

- High Availability: If one container crashes, the balancer redirects traffic to the healthy ones.

- Scalability: You can add more backend "workers" without changing the URL the user sees.

- Performance: It distributes the heavy C++ string transformation tasks across multiple CPU cores.

---

## Load Balancing Fundamentals

### 1. The Round Robin Algorithm (The "Fair Share" Rule)

Round Robin is the most common and simplest method of load balancing.

- The Logic: It works like a dealer at a card table. It gives the 1st request to Server A, the 2nd to Server B, the 3rd to Server C, and then loops back to A.

- Why it works for us: Since our native C++ engine is stateless (it doesn't need to "remember" the user from the previous request), any server can handle any request. So there’s no need for complex "sticky sessions."

### 2. Least Connections (The "Smart" Choice)

Least Connections is a dynamic strategy that looks at the current workload of each backend.

- How it works: NGINX keeps a tally of how many active requests each .NET replica is handling. It sends the next request to the server with the fewest active connections.

- Best For: When requests vary wildly in processing time (e.g., one request is a 10-character string, while another is a 2.1MB block that locks up a C++ thread).

- Mechanical Win: It prevents "clumping," where one server gets stuck with three heavy 5MB conversion tasks while another server sits idle.

### Configuration in NGINX

We can toggle between these strategies by modifying your nginx.conf.

For Round Robin:

This is the default behavior. We don't need to specify a directive.

```Ngnix
upstream backend_servers {
    server backend:8080; # Round Robin is implied
}
```

For Least Connections:

Simply add the least_conn directive at the top of the upstream block.

```Ngnix
upstream backend_servers {
    least_conn;
    server backend:8080;
}
```

#### Performance Comparison

| Strategy          | Behavior        | Impact on M2 P-Cores                                                                                                   |
|-------------------|-----------------|--------------------------------------------------------------------------------------                                  |
| Round Robin       | Fixed sequence  | Excellent for high-speed, small-string throughput. Predictable and low-latency.                                        |
| Least Connections | Load-aware      | Critical for the Batch Processing endpoint. Ensures a 20MB batch does not overwhelm one core while others remain idle. |

### Layer 4 vs Layer 7 Load Balancing

| Feature             | Layer 4 (L4) Load Balancer         | Layer 7 (L7) Load Balancer        |
| ------------------- | ---------------------------------- | --------------------------------- |
| OSI Layer           | Transport Layer                    | Application Layer                 |
| Works With          | TCP / UDP                          | HTTP / HTTPS / WebSockets         |
| Decision Based On   | IP address and port                | URL, headers, cookies, hostnames  |
| Performance         | Faster, lower overhead             | Slightly slower, more intelligent |
| SSL Termination     | Usually no                         | Usually yes                       |
| Content Awareness   | Cannot inspect HTTP content        | Can inspect requests deeply       |
| Routing Capability  | Basic connection routing           | Advanced request routing          |
| Example             | AWS NLB, HAProxy TCP mode          | NGINX, AWS ALB, Traefik           |
| Best Use Case       | Ultra-high throughput, low latency | APIs, microservices, web apps     |
| Sticky Sessions     | Limited                            | Full support                      |
| Path-Based Routing  | No                                 | Yes                               |
| Header Manipulation | No                                 | Yes                               |

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
