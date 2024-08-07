#pragma once

#include <iostream>
#include <cstring> // For memcpy
#include <stdexcept>

class CircularBuffer
{
private:
    char* buffer;
    int capacity;
    int readPos;
    int writePos;
    int dataSize;

public:
    CircularBuffer(int initialCapacity = 512)
        : capacity(initialCapacity), readPos(0), writePos(0), dataSize(0) {
        buffer = new char[capacity];
    }

    ~CircularBuffer() {
        delete[] buffer;
        buffer = nullptr;
    }

    // 获取数据
    bool seek(char* data, int length) {
        if (length > dataSize) {
            //throw std::out_of_range("Not enough data in buffer to read.");
            return false;
        }

        int bytesToEnd = capacity - readPos;
        if (length <= bytesToEnd) {
            // 直接拷贝到末尾
            memcpy(data, buffer + readPos, length);
        }
        else {
            // 跨越缓冲区末尾的情况
            memcpy(data, buffer + readPos, bytesToEnd);
            memcpy(data + bytesToEnd, buffer, length - bytesToEnd);
        }
        return true;
    }

    // 写入数据
    void write(const char* data, int length) {
        if (length > capacity - dataSize) {
            // 自动扩容
            resize(capacity * 2); // 假设扩容为当前容量的两倍
        }

        // 写入数据到环形缓冲区
        int bytesToEnd = capacity - writePos;
        if (length <= bytesToEnd) {
            // 直接拷贝到末尾
            memcpy(buffer + writePos, data, length);
        }
        else {
            // 跨越缓冲区末尾的情况
            memcpy(buffer + writePos, data, bytesToEnd);
            memcpy(buffer, data + bytesToEnd, length - bytesToEnd);
        }

        writePos = (writePos + length) % capacity;
        dataSize += length;
    }

    // 读取数据
    void read(char* data, int length) {
        if (length > dataSize) {
            throw std::out_of_range("Not enough data in buffer to read.");
        }

        int bytesToEnd = capacity - readPos;
        if (length <= bytesToEnd) {
            // 直接拷贝到末尾
            memcpy(data, buffer + readPos, length);
        }
        else {
            // 跨越缓冲区末尾的情况
            memcpy(data, buffer + readPos, bytesToEnd);
            memcpy(data + bytesToEnd, buffer, length - bytesToEnd);
        }

        readPos = (readPos + length) % capacity;
        dataSize -= length;
    }

    // 返回当前缓冲区中的数据长度
    int size() const {
        return dataSize;
    }

private:
    // 扩容操作
    void resize(int newCapacity) {
        char* newBuffer = new char[newCapacity];

        // 复制旧数据到新缓冲区
        if (dataSize > 0) {
            if (writePos > readPos) {
                memcpy(newBuffer, buffer + readPos, dataSize);
            }
            else {
                int bytesToEnd = capacity - readPos;
                memcpy(newBuffer, buffer + readPos, bytesToEnd);
                memcpy(newBuffer + bytesToEnd, buffer, writePos);
            }
        }

        delete[] buffer;
        buffer = newBuffer;
        capacity = newCapacity;
        readPos = 0;
        writePos = dataSize;
    }
};



