#include "TimerWheel.h"

TimerWheel stTimerWheel;


void on_timer(void *data) {
    printf("on_timer-%d,current_time-%d \n",(uint32)*(uint32*)data, stTimerWheel.get_current_time());
}

void add_timer1() {
    for (int i = 0; i < 1000; ++i) {
        uint32 *t1 = new uint32;
        *t1 = 1000+10*i;
        stTimerWheel.add_timer(*t1, on_timer, t1);
    }
}

void add_timer2() {
    for (int i = 0; i < 1000; ++i) {
        uint32* t1 = new uint32;
        *t1 = 1000000 + 10000*i + 8;
        stTimerWheel.add_timer(*t1, on_timer, t1);
    }
}

void add_timer3() {
    for (int i = 0; i < 1000; ++i) {
        uint32* t1 = new uint32;
        *t1 = 800000000 + 10000000 * i + 8;
        stTimerWheel.add_timer(*t1, on_timer, t1);
    }
}

//int main() {
//    std::thread t1(add_timer1);
//    std::thread t2(add_timer2);
//    std::thread t3(add_timer3);
//
//    t1.detach();
//    t2.detach();
//    t3.detach();
//
//    //uint32 t1 = 10;
//    //stTimerWheel.add_timer(t1, on_timer, &t1);
//
//    //uint32 t2 = 1006;
//    //stTimerWheel.add_timer(t2, on_timer, &t2);
//
//    //uint32 t3 = 1000006;
//    //stTimerWheel.add_timer(t3, on_timer, &t3);
//
//    //uint32 t4 = 10000006;
//    //stTimerWheel.add_timer(t4, on_timer, &t4);
//
//    //for (int i=0;i<1000000000;++i) {
//    //    stTimerWheel.tick(1);
//    //}
//
//    stTimerWheel.tick(1000000000);
//
//    getchar();
//
//    return 0;
//}


