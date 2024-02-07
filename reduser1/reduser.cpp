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
boost::asio::io_service service;
boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 11));

void reduser::handle_accept(talk_to_client::ptr client, const boost::system::error_code& err) {
    client->set_reduser_ptr(this);
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_();
    acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}
void reduser::do_accept() {
    talk_to_client::ptr client = talk_to_client::new_();
    acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
}
void reduser::work() {
    //connection_log max = find_max(res);

    //std::cout << "SERVER: " << max.server_num << '\n';
    //std::cout << "COUNT: " << max.connections_count << '\n';
    std::cout << '\n';
    //получение результата
    //отправка результата менеджеру
}
void reduser::start() {
    std::cout << "reduser started ";
    started_ = true;
    readed = 0;
    num_of_mappers = 3;
    do_accept();
}
void reduser::map_update(std::unordered_map<long, long> added_map) {
    readed++;
    map_unification(&res, added_map);
    std::cout << serialize(value_from(res)) << "\n";
    if (readed == 3)
    {
        work();
    }
}
