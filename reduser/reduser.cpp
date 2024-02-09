
#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include "../manager/binary.h"
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
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 11));
#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)
ip::tcp::endpoint ep_manager(ip::address::from_string("127.0.0.1"), 2001);

class reduser;
int count = 0;
void do_accept();
class reader;

class reduser {
public:
    reduser() {
    }
public:

    std::unordered_map<long, long> res;
    connection_log result;
    bool started_;
    int readed;
    int num_of_mappers;
    reader* reader_ptr;

public:/*
    void accept_handler(talk_to_client::ptr client, const boost::system::error_code& err) {
        client->start();
        talk_to_client::ptr new_client = talk_to_client::new_();
        acceptor.async_accept(new_client->sock(), boost::bind(accept_handler, new_client, _1));
    }
    void do_accept() {
        talk_to_client::ptr client = talk_to_client::new_();
        acceptor.async_accept(client->sock(), boost::bind(accept_handler, client, _1));
    }*/

    void start() {
        std::cout << "reduser started ";
        started_ = true;
        readed = 0;
        num_of_mappers = 3;
        do_accept();

    }

    void set_reader(reader* re) {
        reader_ptr = re;
    }
   // void do_accept();
};
void work(reduser* red) {
    connection_log max = find_max(red->res);
    red->result = max;

    std::cout << "SERVER: " << max.server_num << '\n';
    std::cout << "COUNT: " << max.connections_count << '\n';
    std::cout << '\n';
    //получение результата
    //отправка результата менеджеру
}
void map_update(reduser* red, std::unordered_map<long, long> added_map) {
    count++;
    map_unification(&(red->res), added_map);
    std::cout << serialize(value_from(red->res)) << "\n";
    if (count == 3)
    {
        work(red);
    }
}
reduser red_;
class reader {
public:
    std::unordered_map<long, long> buf1;
    std::unordered_map<long, long> buf2;
    std::unordered_map<long, long> buf3;
    void set1(std::unordered_map<long, long> new_buf) {
        buf1 = new_buf;

    }
    void set2(std::unordered_map<long, long> new_buf) {
        buf2 = new_buf;
    }
    void set3(std::unordered_map<long, long> new_buf) {
        buf3 = new_buf;
    }
    std::unordered_map<long, long> get1() {
        return buf1;
    }
    std::unordered_map<long, long> get2() {
        return buf2;
    }
    std::unordered_map<long, long> get3() {
        return buf3;
    }

};
reader re;


class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
public:
    typedef talk_to_client self_type;
    talk_to_client() : sock_(service), started_(false) {}
    // talk_to_client(reduser *red) : sock_(service), started_(false) {
    //     reduse_ptr = red;
    // }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start() {
        std::cout << "started ";
        set_reader(&re);
        started_ = true;
        do_read();
        set_reduser_ptr(&red_);
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
    void set_reduser_ptr(reduser* red) {
        reduser_ptr = red;
    }
    void set_reader(reader* read) {
        read_ptr = read;
    }
private:

    void handler(const boost::system::error_code& ec)
    {
        //std::cout << "Hi";

    }
    void on_read_(const error_code& err, size_t bytes) {
        if (!err) {
            std::cout << "readed" << "\n";
            std::string msg(read_buffer_, bytes);
            //std::cout << msg << "\n\n";
            value map_j = parse(msg);
            std::unordered_map<long, long> map = value_to<std::unordered_map<long, long>>(map_j);
            //std::cout << reduser_ptr;
            map_update(reduser_ptr, map);


            if (count == 3) {
                std::string result = serialize(value_from(reduser_ptr->result));
                do_write_manager(result + "\n");
            }
            /*if (!(read_ptr->get1().size()))
            {
                read_ptr->set1(map);
                map_update(reduser_ptr,map);
            }
            else if (!(read_ptr->get2().size()))
            {
                read_ptr->set2(map);
                map_update(reduser_ptr, map);
            }
            else if (!(read_ptr->get3().size()))
            {
                read_ptr->set3(map);
                map_update(reduser_ptr, map);
            }
            else {
                work(reduser_ptr);
            }*/
            //read_ptr->set(map);

            //  reduse_ptr->map_update(map);
        }
        //   stop();
           //do_read();
    }

    void on_write(const error_code& err, size_t bytes) {

    }
    void do_read() {
        std::cout << "reading" << "\n";
        async_read(sock_, buffer(read_buffer_),
            MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read_, _1, _2));
    }

    void do_write_manager(const std::string& msg) {
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_reducer1.async_write_some(buffer(write_buffer_, msg.size()),
            MEM_FN2(on_write, _1, _2));
    }
    size_t read_complete(const boost::system::error_code& err, size_t bytes) {
        if (err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        // std::cout << read_buffer_ << "|"<< bytes<<"|"<< found<<"**********" << "\n";
        return found ? 0 : 1;
    }


private:
    reduser* reduser_ptr;
    reader* read_ptr;
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;

};

void accept_handler(talk_to_client::ptr client, const boost::system::error_code& err) {
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_();

    acceptor.async_accept(new_client->sock(), boost::bind(accept_handler, new_client, _1));
}
void /*reduser::*/ do_accept(/*reduser* reduser_ptr*/) {

    talk_to_client::ptr client = talk_to_client::new_();
   // client->set_reduser_ptr(/*this*/ );
    acceptor.async_accept(client->sock(), boost::bind(do_accept, client, _1));
}







int main(int argc, char* argv[]) {

    //reduser red;
    //red.set_reader(&re);
    red_.start();
    //talk_to_client::ptr client = talk_to_client::new_();
    //acceptor.async_accept(client->sock(), boost::bind(accept_handler, client, _1));

    service.run();
}