#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <vector>

#include "spsc/spsc_queue.hpp"
#include "spsc/mutex_queue.hpp"

using lfQueue = spsc::SPSC_Queue<int, 65536>;
using mxQueue = mutexq::mutex_Queue<int, 65536>;

using ns = std::chrono::nanoseconds;

constexpr int N = 10000000;

template <typename Queue>
void calculate_latency(Queue& q){
    
    using clock = std::chrono::steady_clock;    
    
    std::vector<clock::time_point> timestamps(N);
    std::vector<long long> latencies;
    latencies.reserve(N);

    std::thread producer([&]() {
        for (int i = 0; i < N; ++i) {
            while (!q.push(i)) {
                std::this_thread::yield();
            }
            timestamps[i] = clock::now();
        }
    });

    std::thread consumer([&]() {
        int value;
        for (int i = 0; i < N; ++i) {
            while (!q.pop(value)) {
                std::this_thread::yield();
            }
            auto now = clock::now();
            auto latency =
                std::chrono::duration_cast<ns>(now - timestamps[value]).count();
            latencies.push_back(latency);
        }
    });

    producer.join();
    consumer.join();

    std::sort(latencies.begin(), latencies.end());

    
    auto p50 = latencies[latencies.size() * 50 / 100];
    auto p99 = latencies[latencies.size() * 99 / 100];
    auto p999 = latencies[latencies.size() * 999 / 1000];

    std::cout << "Latency (ns):\n";
    std::cout << "  p50  = " << p50  << "\n";
    std::cout << "  p99  = " << p99  << "\n";
    std::cout << "  p99.9 = " << p999 << "\n\n";

}

int main(){

    lfQueue q1;
    mxQueue q2;

    std::cout << "Lock-free queue\n";
    calculate_latency(q1);

    std::cout << "Mutex queue\n";
    calculate_latency(q2);

    return 0;
}