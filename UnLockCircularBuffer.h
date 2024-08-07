#pragma once


#include <iostream>
#include <vector>
#include <atomic>
#include <cstring>

template <typename T>
class LockFreeBuffer {
private:
    std::vector<T> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
    size_t maxSize;
    std::atomic<bool> resizing;

public:
    LockFreeBuffer(size_t size) : buffer(size), head(0), tail(0), maxSize(size), resizing(false) {}

    // �򻺳�����д������
    void write(const T* data, size_t len) {
        while (true) {
            size_t currentTail = tail.load(std::memory_order_relaxed);
            size_t currentHead = head.load(std::memory_order_acquire);

            if (currentTail >= currentHead + maxSize) {
                if (!resizing.exchange(true, std::memory_order_relaxed)) {
                    resizeBuffer();
                    resizing.store(false, std::memory_order_relaxed);
                }
                continue;
            }

            size_t writeIndex = currentTail % maxSize;
            size_t availableSpace = maxSize - (currentTail - currentHead);

            if (len <= availableSpace) {
                size_t bytesToWrite = std::min(len, maxSize - writeIndex);
                std::memcpy(&buffer[writeIndex], data, bytesToWrite * sizeof(T));
                data += bytesToWrite;
                len -= bytesToWrite;

                tail.store(currentTail + bytesToWrite, std::memory_order_release);
                return;
            }
            else {
                size_t bytesToWrite = std::min(len, availableSpace);
                std::memcpy(&buffer[writeIndex], data, bytesToWrite * sizeof(T));
                data += bytesToWrite;
                len -= bytesToWrite;

                tail.store(currentTail + bytesToWrite, std::memory_order_release);
            }
        }
    }

    // �ӻ������ж�ȡ����
    size_t read(T* data, size_t len) {
        size_t currentHead = head.load(std::memory_order_relaxed);
        size_t currentTail = tail.load(std::memory_order_acquire);

        size_t bytesRead = 0;

        while (bytesRead < len && currentHead < currentTail) {
            size_t readIndex = currentHead % maxSize;
            size_t bytesToRead = std::min(len - bytesRead, currentTail - currentHead);

            std::memcpy(data + bytesRead, &buffer[readIndex], bytesToRead * sizeof(T));
            bytesRead += bytesToRead;
            currentHead += bytesToRead;
        }

        head.store(currentHead, std::memory_order_release);

        return bytesRead;
    }

    // ��ȡ��ǰ�������е����ݳ���
    size_t size() const {
        size_t currentHead = head.load(std::memory_order_relaxed);
        size_t currentTail = tail.load(std::memory_order_relaxed);
        return currentTail - currentHead;
    }

private:
    // ��չ��������С
    void resizeBuffer() {
        size_t currentSize = maxSize;
        size_t newSize = currentSize * 2;

        std::vector<T> newBuffer(newSize);
        size_t currentHead = head.load(std::memory_order_acquire);
        size_t currentTail = tail.load(std::memory_order_acquire);

        size_t currentBufferLength = currentTail - currentHead;
        size_t writeIndex = currentTail % newSize;

        if (currentBufferLength > 0) {
            size_t bytesToEnd = std::min(currentBufferLength, currentSize - currentHead);
            std::memcpy(&newBuffer[writeIndex], &buffer[currentHead % currentSize], bytesToEnd * sizeof(T));
            if (bytesToEnd < currentBufferLength) {
                std::memcpy(&newBuffer[0], &buffer[(currentHead + bytesToEnd) % currentSize], (currentBufferLength - bytesToEnd) * sizeof(T));
            }
        }

        buffer = std::move(newBuffer);
        head.store(0, std::memory_order_release);
        tail.store(currentBufferLength, std::memory_order_release);
        maxSize = newSize;
    }
};

//int main() {
//    LockFreeBuffer<int> buf(5);  // ����һ����ʼ����Ϊ5�������Զ�����������
//
//    // д������
//    int data[] = { 1, 2, 3, 4, 5 };
//    buf.write(data, sizeof(data) / sizeof(data[0]));
//
//    // ��ȡ��ǰ����������
//    std::cout << "Current buffer size: " << buf.size() << std::endl;
//
//    // ��ȡ����
//    int readData[5] = { 0 };
//    size_t bytesRead = buf.read(readData, sizeof(readData) / sizeof(readData[0]));
//
//    // �����ȡ������
//    std::cout << "Read data: ";
//    for (size_t i = 0; i < bytesRead; ++i) {
//        std::cout << readData[i] << " ";
//    }
//    std::cout << std::endl;
//
//    return 0;
//}
