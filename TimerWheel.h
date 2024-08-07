#pragma once

#include "def.h"


struct TimerNode {
    TimerNode(std::function<void(void*)> callback_, uint32 expiration_, void* timerdata_):callback(callback_), expiration(expiration_), timerdata(timerdata_){}
    // 定时器节点的数据结构，包含定时器到期时间等信息
    std::function<void(void*)> callback;
    uint32 expiration;  // 到期时间点
    void *timerdata;
    TimerNode* next;
    std::atomic<bool> enable = true;
};
typedef std::list<TimerNode*> TimerNodeList;



class TimerWheelNode {
public:
    TimerWheelNode(uint32 wheel_size, uint32 tick_duration_ms) : wheel(wheel_size), wheel_size(wheel_size), tick_duration_ms(tick_duration_ms), current_time(0)
    {
        // 初始化时间轮的每个槽位
        for (uint32 i = 0; i < wheel_size; ++i) {
            wheel[i].store(nullptr);  // 初始为空链表
        }
    }

    TimerNode *add_timer(uint32 expiration, uint32 realexpiration, std::function<void(void*)> callback, void* timerdata) {
        uint32 ticks = expiration / tick_duration_ms;
        uint32 slot = ticks % wheel_size;

        TimerNode* new_timer = new TimerNode(callback, realexpiration, timerdata);
        TimerNode* head = wheel[slot].load();
        do {
            new_timer->next = head;
        } while (!wheel[slot].compare_exchange_weak(head, new_timer));
        return new_timer;
    }

    void tick(uint32 tick = 1) {
        for (uint32 i = 0; i < tick; ++i) {
            ++current_time;
            uint32 slot = current_time % wheel_size;
            TimerNode* expired_timers = wheel[slot].exchange(nullptr);
            while (expired_timers) {
                TimerNode* current = expired_timers;
                expired_timers = expired_timers->next;
                if (current) {
                    if (current->enable) {
                        current->callback(current->timerdata);
                    }
                    delete current;
                }
            }
        }
    }

    size_t tick_list(uint32 tick, TimerNodeList& outList) {
        for (uint32 i = 0; i < tick; ++i) {
            ++current_time;
            uint32 slot = current_time % wheel_size;
            TimerNode* expired_timers = wheel[slot].exchange(nullptr);
            while (expired_timers) {
                TimerNode* current = expired_timers;
                expired_timers = expired_timers->next;
                //
                if (current) {
                    if (current->enable) {
                        outList.push_back(current);
                    }
                    else {
                        delete current;
                        current = nullptr;
                    }
                }
            }
        }
        return outList.size();
    }

private:
    std::vector<std::atomic<TimerNode*>> wheel;  // 时间轮的槽位，原子指针
    uint32 wheel_size;  // 时间轮的大小
    uint32 tick_duration_ms;  // 每个槽位的时间跨度
    std::atomic<uint32> current_time;  // 当前时间
};

// 每一级的定时器数量
#define WHEEL_SIZE 1000
// 总定时器等级
#define WHEEL_LEVEL 3
// 定时器最小尺度 毫秒
#define WHEEL_TICK_NODE_MS 10

// 三层时间轮定时器
// 支持最长时间 1000*1000*1000 = 1157天 最小定时器时间10ms
class TimerWheel {
public:
    TimerWheel(){
        m_stTimerWheel[0] = new TimerWheelNode(WHEEL_SIZE, 1);
        m_stTimerWheel[1] = new TimerWheelNode(WHEEL_SIZE, 1);
        m_stTimerWheel[2] = new TimerWheelNode(WHEEL_SIZE, 1);
    }
    ~TimerWheel(){
        for (int i=0;i< WHEEL_LEVEL;++i) {
            delete m_stTimerWheel[i];
            m_stTimerWheel[i] = nullptr;
        }
    }
public:
    uint32 get_current_time() {
        return current_time;
    }

private:
    uint32 current_time;  // 当前时间
    TimerWheelNode *m_stTimerWheel[WHEEL_LEVEL];

public:
    void add_timer(uint32 expiration, std::function<void(void*)> callback, void* timerdata) {
        uint32 expiration_tmp = expiration/ WHEEL_TICK_NODE_MS;
        if (expiration_tmp <=0&& expiration>0) {
            expiration_tmp = 1;
        }
        if (expiration_tmp / (WHEEL_SIZE * WHEEL_SIZE) > 0) {
            m_stTimerWheel[2]->add_timer(expiration_tmp / (WHEEL_SIZE * WHEEL_SIZE), expiration_tmp, callback, timerdata);
        }
        else if (expiration_tmp / WHEEL_SIZE > 0) {
            m_stTimerWheel[1]->add_timer(expiration_tmp / WHEEL_SIZE, expiration_tmp, callback, timerdata);
        }
        else {
            m_stTimerWheel[0]->add_timer(expiration_tmp, expiration_tmp, callback, timerdata);
        }
    }
    // tick 一次 WHEEL_TICK_NODE_MS 毫秒
    void tick(uint32 tick = 1) {
        uint32 u32TriggerTwoWheel = (current_time % WHEEL_SIZE + tick)/ WHEEL_SIZE;
        uint32 u32TriggerThreeWheel = (current_time % (WHEEL_SIZE * WHEEL_SIZE) + tick) / (WHEEL_SIZE * WHEEL_SIZE);

        current_time += tick;
        uint64 slot = current_time % (WHEEL_SIZE* WHEEL_SIZE* WHEEL_SIZE);

        TimerNodeList stTimerNodeList;
        // 第三层需要降级
        if (u32TriggerThreeWheel > 0) {
            if (m_stTimerWheel[2]->tick_list(u32TriggerThreeWheel, stTimerNodeList) > 0) {
                for (auto &timerNode : stTimerNodeList) {
                    uint32 curTick = timerNode->expiration % (WHEEL_SIZE * WHEEL_SIZE);
                    if (curTick/ WHEEL_SIZE > 0) {
                        // 降级到第二级
                        m_stTimerWheel[1]->add_timer(curTick, timerNode->expiration, timerNode->callback, timerNode->timerdata);
                    }
                    else {
                        // 降级到第一级
                        m_stTimerWheel[0]->add_timer(curTick, timerNode->expiration, timerNode->callback, timerNode->timerdata);
                    }
                    delete timerNode;
                }
                stTimerNodeList.clear();
            }
        }
        // 第二层需要降级
        if (u32TriggerTwoWheel > 0) {
            if (m_stTimerWheel[1]->tick_list(u32TriggerTwoWheel, stTimerNodeList) > 0) {
                for (auto& timerNode : stTimerNodeList) {
                    uint32 curTick = timerNode->expiration % WHEEL_SIZE;
                    // 降级到第一级
                    m_stTimerWheel[0]->add_timer(curTick, timerNode->expiration, timerNode->callback, timerNode->timerdata);
                    delete timerNode;
                }
                stTimerNodeList.clear();
            }
        }
        m_stTimerWheel[0]->tick(tick>= WHEEL_SIZE? WHEEL_SIZE : tick);
    }
};
