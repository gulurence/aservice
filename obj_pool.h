#pragma once
#include <iostream>
#include <vector>
#include <atomic>
#include <memory>
#include <stdexcept>


// ring pool

// just use to one write one read
template <typename T, size_t Cap>
class SafeOneWriteOneReadObjPool
{
public:
    explicit SafeOneWriteOneReadObjPool(size_t initial_capacity = Cap)
        : capacity_(initial_capacity), readIndex_(0), writeIndex_(0) {
        buffer_.resize(initial_capacity);
    }

    bool try_push(const T& value) {
        size_t currentWriteIndex = writeIndex_.load(std::memory_order_acquire);
        size_t nextWriteIndex = (currentWriteIndex + 1) % capacity_;

        if (nextWriteIndex == readIndex_.load(std::memory_order_relaxed)) {
            // Buffer is full, try to resize
            if (!resize(capacity_ * 2)) {
                return false; // Failed to resize
            }
            // Retry push after resizing
            return try_push(value);
        }

        buffer_[currentWriteIndex] = value;
        writeIndex_.store(nextWriteIndex, std::memory_order_release);
        return true;
    }

    bool try_pop(T& value) {
        size_t currentReadIndex = readIndex_.load(std::memory_order_acquire);

        if (currentReadIndex == writeIndex_.load(std::memory_order_relaxed)) {
            // Buffer is empty
            return false;
        }

        value = buffer_[currentReadIndex];
        readIndex_.store((currentReadIndex + 1) % capacity_, std::memory_order_release);
        return true;
    }

private:
    bool resize(size_t newCapacity) {
        std::vector<T> newBuffer(newCapacity);
        size_t count = 0;

        size_t currentReadIndex = readIndex_.load(std::memory_order_acquire);
        size_t currentWriteIndex = writeIndex_.load(std::memory_order_acquire);

        while (currentReadIndex != currentWriteIndex) {
            newBuffer[count++] = buffer_[currentReadIndex];
            currentReadIndex = (currentReadIndex + 1) % capacity_;
        }

        buffer_ = std::move(newBuffer);
        capacity_ = newCapacity;
        readIndex_.store(0, std::memory_order_release);
        writeIndex_.store(count, std::memory_order_release);

        return true;
    }

    std::vector<T> buffer_;
    std::atomic<size_t> capacity_;
    std::atomic<size_t> readIndex_;
    std::atomic<size_t> writeIndex_;
};


// use to multi write read
template <typename T, size_t Cap>
class SafeMultiWriteReadObjPool
{
    std::unique_ptr<T[]> data;
    std::atomic<size_t> head{ 0 };
    std::atomic<size_t> tail{ 0 };
    std::atomic<size_t> write{ 0 };
    size_t capacity;
    std::atomic<bool> resizing{ false };

public:
    SafeMultiWriteReadObjPool(size_t initial_capacity = Cap)
        : data(new T[initial_capacity]), capacity(initial_capacity) {
    }

    SafeMultiWriteReadObjPool(const SafeMultiWriteReadObjPool&) = delete;
    SafeMultiWriteReadObjPool& operator=(const SafeMultiWriteReadObjPool&) = delete;
    SafeMultiWriteReadObjPool& operator=(const SafeMultiWriteReadObjPool&) volatile = delete;

    bool push(const T& val) {
        size_t t, w;
        do {
            t = tail.load(std::memory_order_relaxed);
            if ((t + 1) % capacity == head.load(std::memory_order_acquire)) {
                // Buffer full, attempt to resize
                if (!resize(capacity * 2)) {
                    return false; // Resize failed, buffer still full
                }
                continue; // Retry push after successful resize
            }
        } while (!tail.compare_exchange_weak(t, (t + 1) % capacity, std::memory_order_relaxed));
        data[t] = val;
        do {
            w = t;
        } while (!write.compare_exchange_weak(w, (w + 1) % capacity,
                 std::memory_order_release, std::memory_order_relaxed));
        return true;
    }

    bool pop(T& val) {
        size_t h;
        do {
            h = head.load(std::memory_order_relaxed);
            if (h == write.load(std::memory_order_acquire)) {
                // Buffer empty, cannot pop
                return false;
            }
            val = data[h];
        } while (!head.compare_exchange_strong(h, (h + 1) % capacity,
                 std::memory_order_release, std::memory_order_relaxed));
        return true;
    }

private:
    bool resize(size_t new_capacity) {
        bool expected = false;
        if (!resizing.compare_exchange_strong(expected, true)) {
            return false; // Another thread is already resizing
        }

        // Allocate new buffer
        std::unique_ptr<T[]> new_data(new T[new_capacity]);
        size_t count = 0;
        T value;
        while (pop(value)) {
            new_data[count++] = std::move(value);
        }
        // Update capacity and swap buffers
        capacity = new_capacity;
        data.swap(new_data);
        head.store(0, std::memory_order_relaxed);
        tail.store(count, std::memory_order_release);
        write.store(count, std::memory_order_release);

        resizing.store(false, std::memory_order_release);
        return true;
    }
};
