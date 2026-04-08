# Rate Limiting Algorithms in C++

This repository contains a C++ implementation of commonly used **rate limiting algorithms** used in backend systems to control request throughput and prevent abuse.

The project focuses on **algorithmic correctness, concurrency, and time-based logic**, rather than production frameworks or external dependencies.

---

## Overview

Rate limiting is a fundamental backend concept used to:
- Prevent API abuse
- Protect services from overload
- Ensure fair usage among clients

This project implements multiple rate limiting strategies to understand their trade-offs in terms of accuracy, performance, and memory usage.

---

## Implemented Algorithms

### Fixed Window Counter
- Counts requests within fixed time intervals
- Simple and efficient
- Allows burst traffic at window boundaries

**Use case:** Basic rate limiting with minimal overhead

---

### Token Bucket
- Tokens are refilled at a fixed rate
- Allows controlled bursts while smoothing traffic
- Commonly used in real-world systems (e.g., API gateways)

**Use case:** APIs requiring smooth traffic shaping

---

### Sliding Window
- Tracks timestamps of recent requests
- Most accurate rate limiting strategy
- Higher memory usage compared to other approaches

**Use case:** Strict rate enforcement without boundary spikes

---

## Thread Safety

- Per-client data structures are protected using:
  - `std::mutex`
  - `std::lock_guard`
- Ensures correctness under concurrent access
- Tested using multiple threads simulating concurrent clients

---

## Complexity Analysis

| Algorithm      | Time per Request | Space Complexity |
|---------------|------------------|------------------|
| Fixed Window  | O(1)             | O(N)             |
| Token Bucket  | O(1)             | O(N)             |
| Sliding Window| O(K) (worst)     | O(N × K)         |

- **N** = number of clients  
- **K** = number of requests within the time window  

---

## Technology Stack

- Language: C++ (C++17)
- Standard Library Components:
  - `<chrono>` for time handling
  - `<unordered_map>` for per-client storage
  - `<deque>` for sliding window implementation
  - `<mutex>`, `<thread>` for concurrency

No third-party libraries are used.

---

## How to Build and Run

Compile using `g++`:

```bash
g++ sliding_window_multi_client.cpp -std=c++17 -pthread -o rate_limiter
./rate_limiter
