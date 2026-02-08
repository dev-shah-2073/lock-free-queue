#pragma once

#include <mutex>

namespace mutexq{

    template <typename T, std::size_t capacity>
    class mutex_Queue{
    private:
        std::size_t mPush;
        std::size_t mPop;

        std::mutex mMutex;

        T mBuffer[capacity];

    public:
        mutex_Queue() : mPush {0} , mPop {0} {}

        mutex_Queue(const mutex_Queue&) = delete;
        mutex_Queue& operator=(const mutex_Queue&) = delete;

        bool push(const T& val) noexcept;

        bool pop(T& out) noexcept;

    };


    template <typename T, std::size_t capacity>
    bool mutex_Queue<T, capacity>::push(const T& val) noexcept {
        std::lock_guard<std::mutex> lock(mMutex);

        if ((mPush + 1) % capacity == mPop)
            return false;

        mBuffer[mPush] = val;
        mPush = (mPush + 1) % capacity;

        return true;
    }

    template <typename T, std::size_t capacity>
    bool mutex_Queue<T, capacity>::pop(T& out) noexcept {
        std::lock_guard<std::mutex> lock(mMutex);

        if (mPush == mPop)
            return false;

        out = mBuffer[mPop];
        mPop = (mPop + 1) % capacity;

        return true;
    }

}