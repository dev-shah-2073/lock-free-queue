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
