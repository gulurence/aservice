#include "Session.h"




void ServerSession::DoRead() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length) {
                                if (!ec) {
                                    m_pCircularBuffer->write(data_, length);
                                }
                                else if (ec != error::eof) {
                                    std::cerr << "Error on read: " << ec.message() << std::endl;
                                    socket_.close();
                                    //m_bIsActive = false;
                                }
                            });
}

void ServerSession::DoWrite(char* data, std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data, length),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     DoRead();
                                 }
                                 else {
                                     std::cerr << "Error on write: " << ec.message() << std::endl;
                                     socket_.close();
                                 }
                             });
}




void ClientSession::DoRead() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length) {
                                if (!ec) {
                                    m_pCircularBuffer->write(data_, length);
                                }
                                else if (ec != error::eof) {
                                    std::cerr << "Error on read: " << ec.message() << std::endl;
                                    socket_.close();
                                    //m_bIsActive = false;
                                }
                            });
}

void ClientSession::DoWrite(char* data, std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data, length),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     DoRead();
                                 }
                                 else {
                                     std::cerr << "Error on write: " << ec.message() << std::endl;
                                     socket_.close();
                                 }
                             });
}



















