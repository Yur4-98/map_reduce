
#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include "binary.h"
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
ip::tcp::socket sock_reducer(service);
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 11));
#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)
ip::tcp::endpoint ep_manager(ip::address::from_string("127.0.0.1"), 2001);
ip::tcp::endpoint ep_user(ip::address::from_string("127.0.0.1"), 1001);

class reduser;

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
    void set_reduser_ptr(reduser* red) {
        reduser_ptr = red;
    }

private:

    void handler(const boost::system::error_code& ec)
    {
        //std::cout << "Hi";

    }
    void on_read_(const error_code& err, size_t bytes);

    void on_write(const error_code& err, size_t bytes) {

    }
    void do_read() {
        std::cout << "reading" << "\n";
        async_read(sock_, buffer(read_buffer_),
            MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read_, _1, _2));
    }

    void do_write_manager(const std::string& msg) {
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_reducer.async_write_some(buffer(write_buffer_, msg.size()),
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
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;

};

int count = 0;
void do_accept();

//class talk_to_client;
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
        //do_read();
        stop();
    }
    void do_read() {
        async_read(sock_, buffer(read_buffer_),
            MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
    }
    void do_write(const std::string& msg) {
        if (!started()) return;
        std::copy(msg.begin(), msg.end(), write_buffer_);

        //std::cout << write_buffer_ <<"\n";

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


class reduser {
public:
    reduser() {
    }
public:

    std::unordered_map<long, stat_log> res;
    bool started_;
    int readed;
    int num_of_mappers;
    std::pair <long, long> ansv;

public:

    void start() {
        std::cout << "reduser started ";
        started_ = true;
        readed = 0;
        num_of_mappers = 3;
        do_accept();

    }

    void do_accept();
    void map_update( std::unordered_map<long, stat_log> added_map);
    void work();
    void on_write_manager(const error_code& err, size_t bytes){
    
    }
    int m1[4];
    int m2[4];
    int m3[4];

private:
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];



};
void handler() {}
void reduser::work() {
    //ansv = find_max(this->res);

    std::cout << "SERVER: " << find_max(this->res).first << '\n';
    std::cout << "COUNT: " << find_max(this->res).second.sum_connections << '\n';
    std::cout << '\n';
    int tmp;
    for (int i = 0; i < 4; i++)
    {
        tmp = (m3[i] - m2[i])*(m2[i] - m1[i]);
        if (tmp >0)
        {
            if (m3[i] - m2[i] > 0)
            {
                std::cout << "Server: " << i+1 << " rised." << "\n";
            }
            else
            {
                std::cout << "Server: " << i+1 << " falled." << "\n";
            }
        }else
        std::cout << "Server: " << i+1 << " stranged." << "\n";
    }
    //sock_reducer.async_connect(ep_manager,handler);
    std::string res_str = serialize(value_from(find_max(this->res)));
    //res_str = "rres";
    std::cout << res_str <<"|";
    talk_to_svr::start(ep_user,res_str);
    //отправка результата менеджеру
}
void reduser::map_update(std::unordered_map<long, stat_log> added_map) {
    readed++;
    map_unification(&(this->res), added_map);
    std::cout << serialize(value_from(this->res)) << "\n";
    std::cout << readed << "\n";
    if (this->readed % this->num_of_mappers == 0)
    {
        work();
        //sock_reducer.async_connect(ep_user, handler);
    }
}


void talk_to_client::on_read_(const error_code& err, size_t bytes)
{
    if (!err) {
        std::cout << "readed" << "\n";
        std::string msg(read_buffer_, bytes);
        //std::cout << msg << "\n\n";
        char map_id = msg[0];
        msg = msg.erase(0, 1);
        value map_j = parse(msg);
        std::unordered_map<long, stat_log> map = value_to<std::unordered_map<long, stat_log>>(map_j);
        if (map_id == '1')
        {


            reduser_ptr->m1[0] = 10;
            reduser_ptr->m1[1] = 10;
            reduser_ptr->m1[2] = 10;
            reduser_ptr->m1[3] = 40;
        }
        else if (map_id == '2')
        {
            reduser_ptr->m2[0] = 20;
            reduser_ptr->m2[1] = 20;
            reduser_ptr->m2[2] = 20;
            reduser_ptr->m2[3] = 20;
        }
        else if (map_id == '3')
        {
            reduser_ptr->m3[0] = 40;
            reduser_ptr->m3[1] = 40;
            reduser_ptr->m3[2] = 10;
            reduser_ptr->m3[3] = 10;
        }
        msg = msg.erase(0,1);
        value map_j = parse(msg);
        std::unordered_map<long, stat_log> map = value_to<std::unordered_map<long, stat_log>>(map_j);
        
        
        //std::cout << reduser_ptr;
        reduser_ptr->map_update( map);

    }
       stop();
       //do_read();
}

void reduser::do_accept() {
    std::cout << "---" <<"\n";
    talk_to_client::ptr client = talk_to_client::new_();
    client->set_reduser_ptr(this);
    //client->start();
    //acceptor.async_accept(client->sock(), boost::bind(accept_handler, client, _1));
    acceptor.async_accept(
        client->sock(),

        [this,client](const boost::system::error_code& error) {
            client->start();
            this->do_accept();
            
        }
        
    );
}







int main(int argc, char* argv[]) {
    
    reduser red;
    //red.set_reader(&re);
    red.start();
    //talk_to_client::ptr client = talk_to_client::new_();
    //acceptor.async_accept(client->sock(), boost::bind(accept_handler, client, _1));

    service.run();
}