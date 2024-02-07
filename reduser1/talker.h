#pragma once
#include"Header.h"
#include"reduserh.h"
#include <stdio.h>
#include "../Project without MapReduce/binary.h"
#include "../User1/json_req.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/json/src.hpp>
#include <boost/json.hpp>

class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
    friend reduser;
public:
    typedef talk_to_client self_type;
    talk_to_client();


    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start();
    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }
    void stop();
    boost::asio::ip::tcp::socket& sock() { return sock_; }
    void set_reduser_ptr(reduser* red);
private:
    friend void reduser::map_update(std::unordered_map<long, long>);
    void handler(const boost::system::error_code& ec);
    void on_read_(const error_code& err, size_t bytes);
    void on_write(const error_code& err, size_t bytes);
    void do_read();

    void do_write_manager(const std::string& msg);
    size_t read_complete(const boost::system::error_code& err, size_t bytes);


private:
    reduser* reduse_ptr;
    boost::asio::ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;

};