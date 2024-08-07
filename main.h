#include <iostream>
#include <boost/asio.hpp>
#include <thread>

using namespace boost::asio;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(ip::tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(buffer(data_, max_length),
                                [this, self](boost::system::error_code ec, std::size_t length) {
                                    if (!ec) {
                                        do_write(length);
                                    }
                                    else if (ec != error::eof) {
                                        std::cerr << "Error on read: " << ec.message() << std::endl;
                                        socket_.close();
                                    }
                                });
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        async_write(socket_, buffer(data_, length),
                    [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                        if (!ec) {
                            do_read();
                        }
                        else {
                            std::cerr << "Error on write: " << ec.message() << std::endl;
                            socket_.close();
                        }
                    });
    }

    ip::tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server
{
public:
    Server(boost::asio::io_context& io_context, short port)
        : io_context_(io_context), acceptor_(io_context, ip::tcp::endpoint(ip::tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, ip::tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                else {
                    std::cerr << "Error on accept: " << ec.message() << std::endl;
                }
                do_accept();
            });
    }

    boost::asio::io_context& io_context_;
    ip::tcp::acceptor acceptor_;
};

//int main() {
//    try {
//        boost::asio::io_context io_context;
//
//        // 启动服务器，监听端口 8080
//        Server server(io_context, 8080);
//
//        // 启动 I/O 服务
//        io_context.run();
//    }
//    catch (std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//    }
//
//    return 0;
//}
