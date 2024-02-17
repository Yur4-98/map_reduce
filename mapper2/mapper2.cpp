//#define mappers_count = 3;

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif


#include "../User1/json_req.h"
#include "../Project without MapReduce/binary.h"
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
ip::tcp::socket sock_mapper(service);

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)



ip::tcp::endpoint ep_red(ip::address::from_string("127.0.0.1"), 11);

void handler(const boost::system::error_code& ec)
{
    std::cout << "Hi";

}
class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
    typedef talk_to_client self_type;
    talk_to_client() : user_sock_(service), started_(false) {}
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start() {
        started_ = true;
        std::cout << "hi";
        //sock_mapper.async_connect(ep_map1, handler);
        //sock_mapper.async_connect(ep_map2, handler);
        //sock_mapper.async_connect(ep_map3, handler);

        //sock_mapper.async_connect(ep_red1, handler);

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
    ip::tcp::socket& sock() { return user_sock_; }
private:
    void on_read_user(const error_code& err, size_t bytes) {
        if (!err) {
            sock_mapper.async_connect(ep_red, handler);



            std::string req_str(read_buffer_, bytes);
            std::cout << req_str << "\n";
            
            value req_j = parse(req_str);
            req_json::req req_from_user = value_to<req_json::req>(req_j);
            std::string str = req_from_user.file_name;

            const char* file_name = str.c_str();

            std::unordered_map<long, long> res = mapping(file_name, req_from_user.start, req_from_user.end);
            std::string msg_to_reduce = serialize(value_from(res));


            //msg_to_reduce = msg_to_reduce + "\n";
            msg_to_reduce = "2" + msg_to_reduce + "\n";
            std::cout << msg_to_reduce << "|" << msg_to_reduce.size();;

            std::copy(msg_to_reduce.begin(), msg_to_reduce.end(), write_buffer_);
            sock_mapper.async_write_some(buffer(write_buffer_,
                msg_to_reduce.size()), MEM_FN2(on_write, _1, _2));

            /*
            //std::cout << cd.oper_num;
            //work_map
            sock_mapper.async_write_some(buffer(write_buffer_, msg_to_reduce.size()),
                handler);



            */

        }
        //do_read_user();
        //stop();
    }
    void on_read_reduser(const error_code& err, size_t bytes) {
        if (!err) {
            std::string msg(read_buffer_, bytes);
            // echo message back, and then stop
            do_write(msg + "\n");

        }
        stop();
    }
    void on_write(const error_code& err, size_t bytes) {
        do_read_user();
    }
    void do_read_user() {
        async_read(user_sock_, buffer(read_buffer_),
            MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read_user, _1, _2));
    }
    void do_read_reduser() {
        async_read(user_sock_, buffer(read_buffer_),
            MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read_reduser, _1, _2));
    }
    void do_write(const std::string& msg) {
        std::copy(msg.begin(), msg.end(), write_buffer_);
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

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 102));

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
