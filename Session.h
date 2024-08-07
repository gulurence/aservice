#pragma once

#include "def.h"
#include "cbuffer.h"
#include "info_def.h"
#include <boost/asio.hpp>

enum SessionType {
    SessionTypeServer = 0,
    SessionTypeClient,
};

enum SessionState {
    SessionStateVerify = 0,
    SessionStateLogic,
    SessionStateClose,
    SessionStateFree,

    SessionStateMax,
};

using namespace boost::asio;
using ip::tcp;

class ServerSession : public std::enable_shared_from_this<ServerSession>
{
public:
    ServerSession(tcp::socket socket) : socket_(std::move(socket)) {
        m_bIsActive = true;
        m_pCircularBuffer = new CircularBuffer();
    }

private:
    void DoRead();
    void DoWrite(char* data, std::size_t length);

public:
    void Start(){
        DoRead();
    }
    SessionInfo *GetSessionInfo(){
        return m_pSessionInfo;
    }

public:
    SessionState GetState() {
        return m_enState;
    }
    void SetState(SessionState enState) {
        m_enState = enState;
    }


private:
    bool m_bIsActive = false;
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];

private:
    SessionState m_enState = SessionStateVerify;
    CircularBuffer* m_pCircularBuffer;
    SessionInfo* m_pSessionInfo;
};
typedef std::unordered_map<ServerSession*, ServerSession*> ServerSessionMap;
typedef std::unordered_map<uint64, ServerSession*> ServerSessionUidMap;





class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(tcp::socket socket) : socket_(std::move(socket)) {
        m_bIsActive = true;
        m_pCircularBuffer = new CircularBuffer;
    }

private:
    void DoRead();
    void DoWrite(char* data, std::size_t length);

public:
    void Start() {
        DoRead();
    }
    SessionInfo* GetSessionInfo() {
        return m_pSessionInfo;
    }

public:
    SessionState GetState() {
        return m_enState;
    }
    void SetState(SessionState enState) {
        m_enState = enState;
    }


private:
    bool m_bIsActive = false;
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];

private:
    SessionState m_enState;
    CircularBuffer* m_pCircularBuffer;
    SessionInfo* m_pSessionInfo;
};

typedef std::unordered_map<ClientSession*, ClientSession*> ClientSessionMap;
typedef std::unordered_map<uint64, ClientSession*> ClientSessionUidMap;

