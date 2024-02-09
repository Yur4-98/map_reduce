#pragma once
#define  BOOST_JSON_STACK_BUFFER_SIZE  1024 

#include <iostream>
#include <sstream>
#include <string.h>
#include <boost/json/src.hpp>
#include <boost/json.hpp>
using namespace boost::json;


//void tag_invoke(value_from_tag, value& jv, connection_log const& c)
//{
//    jv = {
//        { "server_num" , c.server_num },
//        { "connections_count", c.connections_count }
//    };
//}
//
//connection_log tag_invoke(value_to_tag< connection_log >, value const& jv)
//{
//    object const& obj = jv.as_object();
//    return connection_log{
//        value_to<int>(obj.at("server_num")),
//        value_to<int>(obj.at("connections_count"))
//    };
//}


namespace req_json {
    
    struct req
    {
        std::string file_name;
        int oper_num;
        long start;
        long end;
    };

    void tag_invoke(value_from_tag, value& jv, req const& c)
    {
        jv = {
            { "file_name" , c.file_name },
            { "oper_num", c.oper_num },
            { "start", c.start },
            { "end", c.end }
        };
    }

    req tag_invoke(value_to_tag< req >, value const& jv)
    {
        object const& obj = jv.as_object();
        return req{
            value_to<std::string>(obj.at("file_name")),
            value_to<int>(obj.at("oper_num")),
            value_to<long>(obj.at("start")),
            value_to<long>(obj.at("end"))
        };
    }

};
/*
int main() {





    req_json::req c{ "1.bin", 0 };
    std::string cc = serialize(value_from(c));      //struct->string
    value jv = value_from(cc);                      //string->json
    std::cout << cc << "\n";
    std::cout << jv;                                
    //req_json::req c1( value_to<req_json::req>(jv) );
    value vv = parse(cc);                           //string->json
    req_json::req cd = value_to<req_json::req>(vv); //json->struct

    std::cout << vv << "\n";
    std::cout << cd.file_name;
    return 0;

}*/