#include <iostream>
#include <coroutine>
#include <chrono>
#include <thread>
//
//template<typename T>
//class SegmentedTask {
//public:
//    struct promise_type {
//        SegmentedTask<T> get_return_object()
//        {
//            return SegmentedTask{ Handle::from_promise(*this) };
//        }
//
//        static std::suspend_never initial_suspend() noexcept { return {}; }
//        static std::suspend_always final_suspend() noexcept { return {}; }
//        std::suspend_always yield_value(std::nullopt_t) noexcept { return {}; }
//
//        void return_value(T value) noexcept
//        {
//            returnValue = value;
//        }
//        static void unhandled_exception() { throw; }
//
//        std::optional<T> returnValue;
//    };
//
//    using Handle = std::coroutine_handle<promise_type>;
//
//    explicit SegmentedTask(const Handle coroutine) : coroutine{ coroutine } {}
//
//    ~SegmentedTask()
//    {
//        if (coroutine) {
//            coroutine.destroy();
//        }
//    }
//
//    SegmentedTask(const SegmentedTask&) = delete;
//    SegmentedTask& operator=(const SegmentedTask&) = delete;
//
//    SegmentedTask(SegmentedTask&& other) noexcept : coroutine(other.coroutine) { other.coroutine = {}; }
//
//    SegmentedTask& operator=(SegmentedTask&& other) noexcept
//    {
//        if (this != &other) {
//            if (coroutine) {
//                coroutine.destroy();
//            }
//            coroutine = other.coroutine;
//            other.coroutine = {};
//        }
//        return *this;
//    }
//
//    void Resume() const { coroutine.resume(); }
//    bool IsFinished() const { return coroutine.promise().returnValue.has_value(); }
//    T GetReturnValue() const { return coroutine.promise().returnValue.value(); }
//
//private:
//    Handle coroutine;
//};
//
//enum class EventType {
//    COMMAND,
//    MESSAGE,
//    ALARM
//};
//
//std::vector<EventType> g_events{ EventType::COMMAND, EventType::ALARM,EventType::MESSAGE };
//std::optional<SegmentedTask<int>> suspended;  // 没有执行完的任务保存在这里
//
//
//int test_run() {
//    return 0;
//}
//
//SegmentedTask<int> ProcessCmd() {
//    for (int i{ 0 }; i < 10; ++i) {
//        std::cout << "ProcessCmd thread ID: " << std::this_thread::get_id() << '\n';
//        std::cout << "Processing step " << i << std::endl;
//        int a = test_run();
//        co_yield std::nullopt;
//        printf("%d \n", a);
//    }
//    co_return 0;
//}
//
//void ProcessMsg()
//{
//    std::cout << "ProcessMsg thread ID: " << std::this_thread::get_id() << '\n';
//    std::cout << "Processing Message" << std::endl;
//}
//
//void ProcessAlm()
//{
//    std::cout << "ProcessAlm thread ID: " << std::this_thread::get_id() << '\n';
//    std::cout << "Processing Alarm" << std::endl;
//}

//#include "obj_pool.h"
//#include "Service.h"
//SafeOneWriteOneReadObjPool<Service*, 500> m_pSession[SessionStateMax];
//
//
//void thread_call() {
//
//    Service* pService = nullptr;
//    if (m_pSession[0].try_pop(pService)) {
//
//
//        //suspended = ProcessCmd(pService);
//    }
//
//}

//
//
//int main()
//{
//    for (auto event : g_events) {
//        switch (event) {
//        case EventType::COMMAND:
//            suspended = ProcessCmd();
//            break;
//        case EventType::MESSAGE:
//            ProcessMsg();
//            break;
//        case EventType::ALARM:
//            ProcessAlm();
//            break;
//        }
//    }
//    while (suspended.has_value() && !suspended->IsFinished()) {
//        suspended->Resume();
//    }
//    if (suspended.has_value()) {
//        std::cout << "Final return: " << suspended->GetReturnValue() << std::endl;
//    }
//    return 0;
//}
