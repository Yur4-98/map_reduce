/*
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
}*/

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include "../Project without MapReduce/binary.h"
#include "../User1/json_req.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/json/src.hpp>
#include <boost/json.hpp>
using namespace boost::asio;
using namespace boost::posix_time;
io_service service;
ip::tcp::socket sock_reducer1(service);
#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)
ip::tcp::endpoint ep_manager(ip::address::from_string("127.0.0.1"), 2001);

ip::tcp::endpoint ep_map1(ip::address::from_string("127.0.0.1"), 101);
ip::tcp::endpoint ep_map2(ip::address::from_string("127.0.0.1"), 102);
ip::tcp::endpoint ep_map3(ip::address::from_string("127.0.0.1"), 103);
class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
    typedef talk_to_client self_type;
    talk_to_client() : map_sock_1(service), map_sock_2(service), map_sock_3(service), started_(false) {}
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start() {
        started_ = true;
        read1 = false;
        read2 = false;
        read3 = false;
        sock_reducer1.async_connect(ep_manager, handler);
        do_read();
    }
    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }
    void stop() {
        if (!started_) return;
        started_ = false;
        map_sock_1.close();
        map_sock_2.close();
        map_sock_3.close();
    }
    ip::tcp::socket& sock1() { return map_sock_1; }
    ip::tcp::socket& sock2() { return map_sock_2; }
    ip::tcp::socket& sock3() { return map_sock_3; }
private:
    void handler(const boost::system::error_code& ec)
    {
        //std::cout << "Hi";

    }
    void on_read_1(const error_code& err, size_t bytes) {
        if (!err) {
            std::string msg(read_buffer_1, bytes);
            value map_j = parse(msg);
            std::unordered_map<long, long> map= value_to<std::unordered_map<long, long>>(map_j);
            map_unification(&res,map);


            read1 = true;
        }
        if (read1 and read2 and read3)
        {
            std::string res_j = serialize(value_from(res));
            do_write_manager(res_j);
        }

        //stop();
        //do_read();
    }
    void on_read_2(const error_code& err, size_t bytes) {
        if (!err) {
            std::string msg(read_buffer_2, bytes);
            value map_j = parse(msg);
            std::unordered_map<long, long> map = value_to<std::unordered_map<long, long>>(map_j);
            map_unification(&res, map);



            read2 = true;
        }
        if (read1 and read2 and read3)
        {
            std::string res_j = serialize(value_from(res));
            do_write_manager(res_j);
        }
        //stop();
        //do_read();
    }
    void on_read_3(const error_code& err, size_t bytes) {
        if (!err) {
            std::string msg(read_buffer_3, bytes);
            value map_j = parse(msg);
            std::unordered_map<long, long> map = value_to<std::unordered_map<long, long>>(map_j);
            map_unification(&res, map);



            read3 = true;
        }
        if (read1 and read2 and read3)
        {
            std::string res_j = serialize(value_from(res));
            do_write_manager(res_j);
        }
        //stop();
        //do_read();
    }

    void on_write(const error_code& err, size_t bytes) {
        //do_write_manager();
        read1 = false;
        read2 = false;
        read3 = false;
    }
    void do_read() {
        if (!read1)
        {
            do_read_map1();
        }
        if (!read1)
        {
            do_read_map2();
        }
        if (!read1)
        {
            do_read_map3();
        }
 
    }
    void do_read_map1() {
        async_read(map_sock_1, buffer(read_buffer_1),
            MEM_FN2(read_complete1, _1, _2), MEM_FN2(on_read_1, _1, _2));
    }
    void do_read_map2() {
        async_read(map_sock_2, buffer(read_buffer_2),
            MEM_FN2(read_complete2, _1, _2), MEM_FN2(on_read_2, _1, _2));
    }
    void do_read_map3() {
        async_read(map_sock_3, buffer(read_buffer_3),
            MEM_FN2(read_complete3, _1, _2), MEM_FN2(on_read_3, _1, _2));
    }
    void do_write_manager(const std::string& msg) {
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_reducer1.async_write_some(buffer(write_buffer_, msg.size()),
            MEM_FN2(on_write, _1, _2));
    }
    size_t read_complete1(const boost::system::error_code& err, size_t bytes) {
        if (err) return 0;
        bool found = std::find(read_buffer_1, read_buffer_1 + bytes, '\n') < read_buffer_1 + bytes;
        return found ? 0 : 1;
    }
    size_t read_complete2(const boost::system::error_code& err, size_t bytes) {
        if (err) return 0;
        bool found = std::find(read_buffer_2, read_buffer_2 + bytes, '\n') < read_buffer_2 + bytes;
        return found ? 0 : 1;
    }
    size_t read_complete3(const boost::system::error_code& err, size_t bytes) {
        if (err) return 0;
        bool found = std::find(read_buffer_3, read_buffer_3 + bytes, '\n') < read_buffer_3 + bytes;
        return found ? 0 : 1;
    }
private:
    ip::tcp::socket map_sock_1;
    ip::tcp::socket map_sock_2;
    ip::tcp::socket map_sock_3;
    enum { max_msg = 1024 };
    char read_buffer_1[max_msg];
    char read_buffer_2[max_msg];
    char read_buffer_3[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    bool read1;
    bool read2;
    bool read3;
    std::unordered_map<long, long> res;
};

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 2001));

void handle_accept(talk_to_client::ptr client, const boost::system::error_code& err) {
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_();
    acceptor.async_accept(new_client->sock1(), boost::bind(handle_accept, new_client, _1));
    acceptor.async_accept(client->sock2(), boost::bind(handle_accept, client, _1));
    acceptor.async_accept(client->sock3(), boost::bind(handle_accept, client, _1));
}


int main(int argc, char* argv[]) {
    talk_to_client::ptr client = talk_to_client::new_();
    acceptor.async_accept(client->sock1(), boost::bind(handle_accept, client, _1));
    acceptor.async_accept(client->sock2(), boost::bind(handle_accept, client, _1));
    acceptor.async_accept(client->sock3(), boost::bind(handle_accept, client, _1));

    service.run();
}