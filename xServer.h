#pragma once

#include "Session.h"
#include "xSingleton.h"


class xServer : public xSingleton<xServer>
{
public:
    ~xServer() {}

public:
    xServer(boost::asio::io_context& io_context, short port)
        : m_stAcceptor(m_stIoContext, tcp::endpoint(tcp::v4(), port)),
        m_stSocket(m_stIoContext) {
        DoAccept();
    }

private:
    void DoAccept();

    tcp::acceptor m_stAcceptor;
    tcp::socket m_stSocket;

private:
    uint16 m_u16Port = 0;
    boost::asio::io_context m_stIoContext;
};

class xClientServer : public xSingleton<xServer>
{
public:
    ~xClientServer() {}

public:
    xClientServer(boost::asio::io_context& io_context, short port)
        : m_stAcceptor(m_stIoContext, tcp::endpoint(tcp::v4(), port)),
        m_stSocket(m_stIoContext) {
        DoAccept();
    }

private:
    void DoAccept();

    tcp::acceptor m_stAcceptor;
    tcp::socket m_stSocket;

private:
    uint16 m_u16Port = 0;
    boost::asio::io_context m_stIoContext;
};

class GameServer : public xSingleton<GameServer>
{
public:


private:


public:
    int StartNet(uint16 u16Port);
};