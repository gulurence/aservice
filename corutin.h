#pragma once

#include <coroutine>
#include <chrono>
#include <thread>

template<typename T>
class SegmentedTask {
public:
    struct promise_type {
        SegmentedTask<T> get_return_object()
        {
            return SegmentedTask{ Handle::from_promise(*this) };
        }

        static std::suspend_never initial_suspend() noexcept { return {}; }
        static std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(std::nullopt_t) noexcept { return {}; }

        void return_value(T value) noexcept
        {
            returnValue = value;
        }
        static void unhandled_exception() { throw; }

        std::optional<T> returnValue;
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit SegmentedTask(const Handle coroutine) : coroutine{ coroutine } {}

    ~SegmentedTask()
    {
        if (coroutine) {
            coroutine.destroy();
        }
    }

    SegmentedTask(const SegmentedTask&) = delete;
    SegmentedTask& operator=(const SegmentedTask&) = delete;

    SegmentedTask(SegmentedTask&& other) noexcept : coroutine(other.coroutine) { other.coroutine = {}; }

    SegmentedTask& operator=(SegmentedTask&& other) noexcept
    {
        if (this != &other) {
            if (coroutine) {
                coroutine.destroy();
            }
            coroutine = other.coroutine;
            other.coroutine = {};
        }
        return *this;
    }

    void Resume() const { coroutine.resume(); }
    bool IsFinished() const { return coroutine.promise().returnValue.has_value(); }
    T GetReturnValue() const { return coroutine.promise().returnValue.value(); }

private:
    Handle coroutine;
};
