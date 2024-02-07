#pragma once
#include"Header.h"
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
class reduser {
     friend talk_to_client;
public:
    reduser() {
    }
private:

    std::unordered_map<long, long> res;
    bool started_;
    int readed;
    int num_of_mappers;

public:
    void handle_accept(talk_to_client::ptr client, const boost::system::error_code& err) {}
    void do_accept() {}
    void work() {    }
    void start() {}
    void map_update(std::unordered_map<long, long> added_map) {}
};