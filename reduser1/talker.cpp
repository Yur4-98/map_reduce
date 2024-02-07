#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif
#include "reduserh.h"
#include "talker.h"
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
#include <iostream>
#include <boost/thread.hpp>
#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)
boost::asio::io_service service;
boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 11));

talk_to_client::talk_to_client() : sock_(service), started_(false) {}

void talk_to_client::start() {
    std::cout << "started ";
    started_ = true;
    do_read();
}

void talk_to_client::stop() {
    if (!started_) return;
    started_ = false;
    sock_.close();
}
boost::asio::ip::tcp::socket& talk_to_client::sock() { return sock_; }
void talk_to_client::set_reduser_ptr(reduser* red) {
    reduse_ptr = red;
}


void talk_to_client::handler(const boost::system::error_code& ec)
{
    //std::cout << "Hi";

}
void talk_to_client::on_read_(const error_code& err, size_t bytes) {
    if (!err) {
        std::cout << "readed" << "\n";
        std::string msg(read_buffer_, bytes);
        std::cout << msg << "\n";
        value map_j = parse(msg);
        std::unordered_map<long, long> map = value_to<std::unordered_map<long, long>>(map_j);
        reduse_ptr->map_update(map);
    }
    stop();
    //do_read();
}

void talk_to_client::on_write(const error_code& err, size_t bytes) {

}
void talk_to_client::do_read() {
    std::cout << "read" << "\n";
    async_read(sock_, boost::asio::buffer(read_buffer_),
        MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read_, _1, _2));
}
/*
void talk_to_client::do_write_manager(const std::string& msg) {
    std::copy(msg.begin(), msg.end(), write_buffer_);
    sock_reducer1.async_write_some(boost::asio::buffer(write_buffer_, msg.size()),
        MEM_FN2(on_write, _1, _2));
}*/
size_t talk_to_client::read_complete(const boost::system::error_code& err, size_t bytes) {
    if (err) return 0;
    bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
    return found ? 0 : 1;
}