# Lock-Free vs Mutex Queue (C++)

A minimal, high-performance comparison between a single-producer single-consumer (SPSC) lock-free queue and a mutex-based queue, with tail-latency (p50 / p99 / p99.9) benchmarking.

This project is intended as a learning + systems showcase for low-latency C++ programming.

## Project Tree
<pre>
lock-free-queue/
|
|-- spsc/
|   |-- spsc_queue.hpp          # lock-free queue class
|   |-- mutex_queue.hpp         # mutex queue class
|
|-- benchmark.cpp               # For benchmarking
|-- README.md                   # Project documentation
</pre>

## Performance Comparison: Lock-Free Queue vs Mutex Queue

This project compares a Single-Producer Single-Consumer (SPSC) lock-free queue with a traditional mutex-based queue, focusing on end-to-end latency under concurrent producer–consumer workloads.

### Test Results

All latencies are measured in nanoseconds (ns). <br/>

<pre>
Lock-free queue
Latency (ns):
  p50   = 5,862,958
  p99   = 6,180,416
  p99.9 = 6,243,375

Mutex queue
Latency (ns):
  p50   = 16,099,375
  p99   = 19,314,792
  p99.9 = 19,735,000
</pre>
