#pragma once

#include "def.h"


struct TimerNode {
    TimerNode(std::function<void(void*)> callback_, uint32 expiration_, void* timerdata_):callback(callback_), expiration(expiration_), timerdata(timerdata_){}
    // ��ʱ���ڵ�����ݽṹ��������ʱ������ʱ�����Ϣ
    std::function<void(void*)> callback;
    uint32 expiration;  // ����ʱ���
    void *timerdata;
    TimerNode* next;
    std::atomic<bool> enable = true;
};
typedef std::list<TimerNode*> TimerNodeList;



class TimerWheelNode {
public:
    TimerWheelNode(uint32 wheel_size, uint32 tick_duration_ms) : wheel(wheel_size), wheel_size(wheel_size), tick_duration_ms(tick_duration_ms), current_time(0)
    {
        // ��ʼ��ʱ���ֵ�ÿ����λ
        for (uint32 i = 0; i < wheel_size; ++i) {
            wheel[i].store(nullptr);  // ��ʼΪ������
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
    std::vector<std::atomic<TimerNode*>> wheel;  // ʱ���ֵĲ�λ��ԭ��ָ��
    uint32 wheel_size;  // ʱ���ֵĴ�С
    uint32 tick_duration_ms;  // ÿ����λ��ʱ����
    std::atomic<uint32> current_time;  // ��ǰʱ��
};

// ÿһ���Ķ�ʱ������
#define WHEEL_SIZE 1000
// �ܶ�ʱ���ȼ�
#define WHEEL_LEVEL 3
// ��ʱ����С�߶� ����
#define WHEEL_TICK_NODE_MS 10

// ����ʱ���ֶ�ʱ��
// ֧���ʱ�� 1000*1000*1000 = 1157�� ��С��ʱ��ʱ��10ms
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
    uint32 current_time;  // ��ǰʱ��
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
    // tick һ�� WHEEL_TICK_NODE_MS ����
    void tick(uint32 tick = 1) {
        uint32 u32TriggerTwoWheel = (current_time % WHEEL_SIZE + tick)/ WHEEL_SIZE;
        uint32 u32TriggerThreeWheel = (current_time % (WHEEL_SIZE * WHEEL_SIZE) + tick) / (WHEEL_SIZE * WHEEL_SIZE);

        current_time += tick;
        uint64 slot = current_time % (WHEEL_SIZE* WHEEL_SIZE* WHEEL_SIZE);

        TimerNodeList stTimerNodeList;
        // ��������Ҫ����
        if (u32TriggerThreeWheel > 0) {
            if (m_stTimerWheel[2]->tick_list(u32TriggerThreeWheel, stTimerNodeList) > 0) {
                for (auto &timerNode : stTimerNodeList) {
                    uint32 curTick = timerNode->expiration % (WHEEL_SIZE * WHEEL_SIZE);
                    if (curTick/ WHEEL_SIZE > 0) {
                        // �������ڶ���
                        m_stTimerWheel[1]->add_timer(curTick, timerNode->expiration, timerNode->callback, timerNode->timerdata);
                    }
                    else {
                        // ��������һ��
                        m_stTimerWheel[0]->add_timer(curTick, timerNode->expiration, timerNode->callback, timerNode->timerdata);
                    }
                    delete timerNode;
                }
                stTimerNodeList.clear();
            }
        }
        // �ڶ�����Ҫ����
        if (u32TriggerTwoWheel > 0) {
            if (m_stTimerWheel[1]->tick_list(u32TriggerTwoWheel, stTimerNodeList) > 0) {
                for (auto& timerNode : stTimerNodeList) {
                    uint32 curTick = timerNode->expiration % WHEEL_SIZE;
                    // ��������һ��
                    m_stTimerWheel[0]->add_timer(curTick, timerNode->expiration, timerNode->callback, timerNode->timerdata);
                    delete timerNode;
                }
                stTimerNodeList.clear();
            }
        }
        m_stTimerWheel[0]->tick(tick>= WHEEL_SIZE? WHEEL_SIZE : tick);
    }
};
