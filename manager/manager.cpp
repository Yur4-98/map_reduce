#define mappers_count = 3;

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


class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
    typedef talk_to_client self_type;
    talk_to_client() : sock_(service), started_(false) {}
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start() {
        started_ = true;
        do_read();
    }
    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }
    void stop() {
        if (!started_) return;
        started_ = false;
        sock_.close();
    }
    ip::tcp::socket& sock() { return sock_; }
private:
    void on_read(const error_code& err, size_t bytes) {
        if (!err) {
            
            
            
    
            //std::string req_str(read_buffer_, bytes);
    
           // value req_j = parse_str(req_str);
            //req_json::req req = value_to<req_json::req>(req_j);
  
            //std::cout << cd.oper_num;
            //std::string str = req.file_path;
            //const char* cstr = str.c_str();
            /////////make_interval(count_num(cstr), mappers_count)

            //////mapper_connect1/2/3
            //////mapper_write1/2/3
            //////reduser_read

            std::string msg(read_buffer_, bytes);
            // echo message back, and then stop
            do_write(msg + "\n");
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
};

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

void handle_accept(talk_to_client::ptr client, const boost::system::error_code& err) {
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_();
    acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}


int main(int argc, char* argv[]) {
    talk_to_client::ptr client = talk_to_client::new_();
    acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));



    service.run();
}
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

ip::tcp::endpoint ep(ip::tcp::v4(), 2001);
ip::tcp::acceptor acc(service, ep);
void handle_accept(socket_ptr sock, const boost::system::error_code& err);
void connect_handler(const boost::system::error_code& ec);
void start_accept(socket_ptr sock);
ip::tcp::socket sock_manager(service);
void handle_accept(socket_ptr sock, const boost::system::error_code& err)
{
    if (err) return;
    // at this point, you can read/write to the socket
    std::cout << "Accepted";
    socket_ptr sock1(new ip::tcp::socket(service));
    start_accept(sock1);

    ip::tcp::endpoint ep_map1(ip::address::from_string("127.0.0.1"), 2000);
    sock_manager.async_connect(ep_map1, connect_handler);

}
void start_accept(socket_ptr sock)
{
    acc.async_accept(*sock, boost::bind(handle_accept, sock, _1));
}
void connect_handler(const boost::system::error_code& ec)
{
    std::cout << "Hi";
    
}

int main() {
    //socket_ptr sockets[];
    
    socket_ptr sock(new ip::tcp::socket(service));

    start_accept(sock);
    service.run();
}
*/