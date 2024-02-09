#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif


#include "json_req.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <boost/noncopyable.hpp>
using namespace boost::asio;
io_service service;
ip::tcp::socket sock_user(service);
#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)


class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
    typedef talk_to_client self_type;
    talk_to_client() : user_sock_(service), started_(false) {}
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start() {
        started_ = true;

        do_read_user();
    }
    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }
    void stop() {
        if (!started_) return;
        started_ = false;
        user_sock_.close();
    }
    ip::tcp::socket& sock() {
        return user_sock_;
    }

private:

    void on_read_user(const error_code& err, size_t bytes) {
        

        if (!err) {
            std::string req_str(read_buffer_, bytes - 1);
            std::cout << req_str << "\n";

            value req_j = parse(req_str);
            std::pair <long, long> req_from_reduser = value_to<std::pair <long, long>>(req_j);

            std::cout << "SERVER: " << req_from_reduser.first << '\n';
            std::cout << "COUNT: " << req_from_reduser.second << '\n';
            std::cout << '\n';
       
        }
        stop();
    }

    void on_read_reduser(const error_code& err, size_t bytes) {
        if (!err) {
            std::string msg(read_buffer_, bytes);
            // echo message back, and then stop
            std::cout << msg << "\n";
            do_write(msg + "\n");

        }
        stop();
    }
    void on_write(const error_code& err, size_t bytes) {
        do_read_user();
    }
    void on_write_map1(const error_code& err, size_t bytes) {
        // std::cout << "11";
    }
    void do_read_user() {
        std::cout << read_buffer_;
        async_read(user_sock_, buffer(read_buffer_),
            MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read_user, _1, _2));
    }
    /*void do_read_reduser() {
        async_read(user_sock_, buffer(read_buffer_),
            MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read_reduser, _1, _2));
    }*/
    void do_write(const std::string& msg) {
        std::copy(msg.begin(), msg.end(), write_buffer_);
        // std::cout <<write_buffer_ <<"|" << "\n";
        user_sock_.async_write_some(buffer(write_buffer_, msg.size()),
            MEM_FN2(on_write, _1, _2));
    }


    size_t read_complete(const boost::system::error_code& err, size_t bytes) {
        if (err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }
private:

    ip::tcp::socket user_sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
};

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 1001));

void handle_accept(talk_to_client::ptr client, const boost::system::error_code& err) {
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_();
    acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}
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
            std::cout << copy;
            //std::cout << "server echoed our " << message_ << ": "
            //    << (copy == message_ ? "OK" : "FAIL") << std::endl;
        }
        //stop();
    }

    void on_write(const error_code& err, size_t bytes) {
    std::cout << "writed" << "\n";
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
    
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    
    
    req_json::req req1{ "E:\\paralelprog\\test\\Project without MapReduce\\time_serie1.txt", 0,0,0 };//задать путь к файлу
    std::string req1_str = serialize(value_from(req1));
    
    
    std::string message = req1_str;
    
    talk_to_svr::start(ep, message);
    boost::this_thread::sleep(boost::posix_time::millisec(100));

    talk_to_client::ptr client = talk_to_client::new_();
    acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));

    service.run();
}