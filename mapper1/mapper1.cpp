/*
#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif


#include "../User1/json_req.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <boost/noncopyable.hpp>
using namespace boost::asio;

void connect_handler(const boost::system::error_code& ec)
{
    std::cout << "Hi";
}


void read_handler(const boost::system::error_code& e, size_t bytes)
{

}
void write_handler(const boost::system::error_code& e, size_t bytes) 
{

}


int main() 
{

    io_service service;
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    ip::tcp::socket sock(service);

    sock.async_connect(ep, connect_handler);

   // std::string str1 = "111";
   //sock.async_read_some(buffer(str1),read_handler);
   //sock.async_write_some(buffer(str1), read_handler);
   



    service.run();
    

}*/

/*
#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include "../User1/json_req.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
io_service service;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_svr : public boost::enable_shared_from_this<talk_to_svr>
    , boost::noncopyable {
    typedef talk_to_svr self_type;
    talk_to_svr(const std::string& message)
        : sock_(service), started_(true), message_(message) {}
    void start(ip::tcp::endpoint ep) {
        sock_.async_connect(ep, MEM_FN1(on_connect, _1));
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_svr> ptr;

    static ptr start(ip::tcp::endpoint ep, const std::string& message) {
        ptr new_(new talk_to_svr(message));
        new_->start(ep);
        return new_;
    }
    void stop() {
        if (!started_) return;
        started_ = false;
        sock_.close();
    }
    bool started() { return started_; }
private:
    void on_connect(const error_code& err) {
        if (!err)      do_write(message_ + "\n");
        else            stop();
    }
    void on_read(const error_code& err, size_t bytes) {
        if (!err) {
            std::string copy(read_buffer_, bytes - 1);
            std::cout << "server echoed our " << message_ << ": "
                << (copy == message_ ? "OK" : "FAIL") << std::endl;
        }
        stop();
    }

    void on_write(const error_code& err, size_t bytes) {
        do_read();
    }
    void do_read() {
        async_read(sock_, buffer(read_buffer_),
            MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
    }
    void do_write(const std::string& msg) {
        if (!started()) return;
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some(buffer(write_buffer_, msg.size()),
            MEM_FN2(on_write, _1, _2));
    }
    size_t read_complete(const boost::system::error_code& err, size_t bytes) {
        if (err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string message_;
};

int main(int argc, char* argv[]) {
    // connect several clients
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
    const char* messages[] = { "John says hi", "so does James", "Lucy just got home", 0 };
    for (const char** message = messages; *message; ++message) {
        talk_to_svr::start(ep, *message);
        boost::this_thread::sleep(boost::posix_time::millisec(100));
    }
    service.run();
}*/


#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif


#include "../User1/json_req.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using namespace boost::posix_time;
io_service service;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)



using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

ip::tcp::endpoint ep(ip::tcp::v4(), 2000);
ip::tcp::acceptor acc(service, ep);
void handle_accept(socket_ptr sock, const boost::system::error_code& err);
void start_accept(socket_ptr sock);

void handle_accept(socket_ptr sock, const boost::system::error_code& err)
{
    if (err) return;
    // at this point, you can read/write to the socket
    std::cout << "Accepted";
    socket_ptr sock1(new ip::tcp::socket(service));
    start_accept(sock1);
}
void start_accept(socket_ptr sock)
{
    acc.async_accept(*sock, boost::bind(handle_accept, sock, _1));
}


int main() {
    //socket_ptr sockets[];

    socket_ptr sock(new ip::tcp::socket(service));
    start_accept(sock);
    service.run();
}