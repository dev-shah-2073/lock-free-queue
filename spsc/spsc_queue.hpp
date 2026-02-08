#pragma once

// #include <new>
#include <atomic>

namespace spsc {

    template <typename T, std::size_t capacity>
    class SPSC_Queue{
    private:
        static constexpr std::size_t cache_line = 64; // std::hardware_destructive_interference_size;

        alignas(cache_line) std::atomic<std::size_t> mPush;     // written by producer
        alignas(cache_line) std::atomic<std::size_t> mPop;      // written by consumer
        
        // This does NOT put every element on its own cache line
        alignas(cache_line) T mBuffer[capacity]; 

    public:
        SPSC_Queue() : mPush {0}, mPop {0} {}

        SPSC_Queue(const SPSC_Queue&) = delete;
        SPSC_Queue& operator=(const SPSC_Queue&) = delete;

        bool push(const T& val) noexcept;

        bool pop(T& out) noexcept;
    };


    template <typename T, std::size_t capacity>
    bool SPSC_Queue<T, capacity>::push(const T& val) noexcept {
        std::size_t push = mPush.load(std::memory_order_relaxed);
        std::size_t next = (push + 1) % capacity;
            
        // check if full
        if (next == mPop.load(std::memory_order_acquire))
            return false;

        mBuffer[push] = val;
            
        mPush.store(next, std::memory_order_release);
        return true;
    }


    template <typename T, std::size_t capacity>
    bool SPSC_Queue<T, capacity>::pop(T& out) noexcept {
        std::size_t pop = mPop.load(std::memory_order_relaxed);
            
        // check if empty
        if (pop == mPush.load(std::memory_order_acquire))
            return false;

        out = mBuffer[pop];
            
        mPop.store((pop + 1) % capacity, std::memory_order_release);
        return true;
    }

}

// template <typename T, std::size_t capacity>
// void producer(SPSC_Queue<T, capacity>& queue){
    
//     std::vector <T> arr(capacity);
//     for (int i=0; i<capacity; i++) arr[i] = i;

//     for (int i=0; i<capacity; ++i){
        
//         while (!queue.push(arr[i])){
//             std::this_thread::yield();
//         }

//         std::cout << "Produced : " << arr[i]<< "\n";
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));

//     }

// }

// template <typename T, std::size_t capacity>
// void consumer(SPSC_Queue<T, capacity>& queue){

//     T val;

//     for (int i=0; i<capacity; i++){
        
//         while (!queue.pop(val)){
//             std::this_thread::yield();
//         }

//         std::cout << "Consumed : " << val << "\n";
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));

//     }

// }

// int main(){
//     SPSC_Queue<int, 16> q;

//     std::thread prod(
//         [&q] {
//             producer<int, 16> (q);
//         }
//     );

//     std::thread cons(
//         [&q] {
//             consumer<int, 16> (q);
//         }
//     );

//     prod.join();
//     cons.join();

//     return 0;
// }
