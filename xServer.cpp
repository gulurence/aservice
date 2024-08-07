#include "xServer.h"

#include "SessionMgr.h"



void xServer::DoAccept() {
    m_stAcceptor.async_accept(m_stSocket,
                           [this](boost::system::error_code ec) {
                               if (!ec) {
                                   //Session* pSession = new Session(std::move(m_stSocket));
                                   //SessionMgr::getMe().AddToVerify(pSession);
                                   //pSession->Start();
                               }

                               DoAccept();
                           });
}

void xClientServer::DoAccept() {
    m_stAcceptor.async_accept(m_stSocket,
                              [this](boost::system::error_code ec) {
                                  if (!ec) {
                                      //Session* pSession = new Session(std::move(m_stSocket));
                                      //SessionMgr::getMe().AddToVerify(pSession);
                                      //pSession->Start();
                                  }

                                  DoAccept();
                              });
}




int GameServer::StartNet(uint16 u16Port){

    try {
        boost::asio::io_context *pIoContext = new boost::asio::io_context();
        xServer *pServer = new xServer(*pIoContext, u16Port);
        pIoContext->run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }


    return 0;

}