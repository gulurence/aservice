#include "obj_pool.h"

#include <thread>

//
//void wtest1(LockFreeRingBuffer<int> *pool){
//
//    // Push some values into the buffer
//    for (int i = 0; i < 1000000; ++i) {
//        if (pool->try_push(i)) {
//            //std::cout << "Pushed " << i << " into buffer." << std::endl;
//        }
//        else {
//            std::cout << "Buffer is full. Failed to push " << i << "." << std::endl;
//        }
//    }
//}
//
//void wtest2(LockFreeRingBuffer<int>* pool){
//
//    // Push some values into the buffer
//    for (int i = 0; i < 1000000; ++i) {
//        if (pool->try_push(i)) {
//            std::cout << "Pushed " << i << " into buffer." << std::endl;
//        }
//        else {
//            std::cout << "Buffer is full. Failed to push " << i << "." << std::endl;
//        }
//    }
//}
//
//void rtest1(LockFreeRingBuffer<int>* pool) {
//
//    int value;
//    while (pool->try_pop(value)) {
//        std::cout << "Popped " << value << " from buffer." << std::endl;
//    }
//}
//
//void rtest2(LockFreeRingBuffer<int>* pool) {
//    while (true)
//    {
//        int value;
//        while (pool->try_pop(value)) {
//            std::cout << "Popped " << value << " from buffer." << std::endl;
//        }
//    }
//}
//
//
//int main() {
//    LockFreeRingBuffer<int> buffer(4);
//
//
//    std::thread t1(wtest1,&buffer);
//    //std::thread t2(wtest2, &buffer);
//
//    std::thread t3(rtest1, &buffer);
//    //std::thread t4(rtest2, &buffer);
//
//    t1.detach();
//    //t2.detach();
//    t3.detach();
//    //t4.detach();
//
//    //// Push some values into the buffer
//    //for (int i = 0; i < 8; ++i) {
//    //    if (buffer.try_push(i)) {
//    //        std::cout << "Pushed " << i << " into buffer." << std::endl;
//    //    }
//    //    else {
//    //        std::cout << "Buffer is full. Failed to push " << i << "." << std::endl;
//    //    }
//    //}
//
//    //// Pop all values from the buffer
//    //int value;
//    //while (buffer.try_pop(value)) {
//    //    std::cout << "Popped " << value << " from buffer." << std::endl;
//    //}
//
//    getchar();
//
//    return 0;
//}
//


//ring_buffer<int, 1000> pool;
//void wtest1() {
//
//    // Push some values into the buffer
//    for (int i = 0; i < 100000; ++i) {
//        if (pool.push(i)) {
//            //std::cout << "Pushed " << i << " into buffer." << std::endl;
//        }
//        else {
//            std::cout << "Buffer is full. Failed to push " << i << "." << std::endl;
//        }
//    }
//}
//
//void wtest2() {
//
//    // Push some values into the buffer
//    for (int i = 100000; i < 2000000; ++i) {
//        if (pool.push(i)) {
//            //std::cout << "Pushed " << i << " into buffer." << std::endl;
//        }
//        else {
//            std::cout << "Buffer is full. Failed to push " << i << "." << std::endl;
//        }
//    }
//}
//
//void rtest1() {
//
//    while (true) {
//        int value;
//        while (pool.pop(value)) {
//            std::cout << "1 Popped " << value << " from buffer." << std::endl;
//        }
//    }
//}
//
//void rtest2() {
//    while (true) {
//        int value;
//        while (pool.pop(value)) {
//            std::cout << "2 Popped " << value << " from buffer." << std::endl;
//        }
//    }
//}
//
//
//int main() {
//
//
//    std::thread t1(wtest1);
//    std::thread t2(wtest2);
//
//    std::thread t3(rtest1);
//    std::thread t4(rtest2);
//
//    t1.detach();
//    t2.detach();
//    t3.detach();
//    t4.detach();
//
//    //// Push some values into the buffer
//    //for (int i = 0; i < 8; ++i) {
//    //    if (buffer.try_push(i)) {
//    //        std::cout << "Pushed " << i << " into buffer." << std::endl;
//    //    }
//    //    else {
//    //        std::cout << "Buffer is full. Failed to push " << i << "." << std::endl;
//    //    }
//    //}
//
//    //// Pop all values from the buffer
//    //int value;
//    //while (buffer.try_pop(value)) {
//    //    std::cout << "Popped " << value << " from buffer." << std::endl;
//    //}
//
//    getchar();
//
//
//
//
//    return 0;
//}













