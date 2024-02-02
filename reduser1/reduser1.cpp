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


}