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

    // ��ȡ����
    bool seek(char* data, int length) {
        if (length > dataSize) {
            //throw std::out_of_range("Not enough data in buffer to read.");
            return false;
        }

        int bytesToEnd = capacity - readPos;
        if (length <= bytesToEnd) {
            // ֱ�ӿ�����ĩβ
            memcpy(data, buffer + readPos, length);
        }
        else {
            // ��Խ������ĩβ�����
            memcpy(data, buffer + readPos, bytesToEnd);
            memcpy(data + bytesToEnd, buffer, length - bytesToEnd);
        }
        return true;
    }

    // д������
    void write(const char* data, int length) {
        if (length > capacity - dataSize) {
            // �Զ�����
            resize(capacity * 2); // ��������Ϊ��ǰ����������
        }

        // д�����ݵ����λ�����
        int bytesToEnd = capacity - writePos;
        if (length <= bytesToEnd) {
            // ֱ�ӿ�����ĩβ
            memcpy(buffer + writePos, data, length);
        }
        else {
            // ��Խ������ĩβ�����
            memcpy(buffer + writePos, data, bytesToEnd);
            memcpy(buffer, data + bytesToEnd, length - bytesToEnd);
        }

        writePos = (writePos + length) % capacity;
        dataSize += length;
    }

    // ��ȡ����
    void read(char* data, int length) {
        if (length > dataSize) {
            throw std::out_of_range("Not enough data in buffer to read.");
        }

        int bytesToEnd = capacity - readPos;
        if (length <= bytesToEnd) {
            // ֱ�ӿ�����ĩβ
            memcpy(data, buffer + readPos, length);
        }
        else {
            // ��Խ������ĩβ�����
            memcpy(data, buffer + readPos, bytesToEnd);
            memcpy(data + bytesToEnd, buffer, length - bytesToEnd);
        }

        readPos = (readPos + length) % capacity;
        dataSize -= length;
    }

    // ���ص�ǰ�������е����ݳ���
    int size() const {
        return dataSize;
    }

private:
    // ���ݲ���
    void resize(int newCapacity) {
        char* newBuffer = new char[newCapacity];

        // ���ƾ����ݵ��»�����
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



