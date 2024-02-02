#pragma once
#define  BOOST_JSON_STACK_BUFFER_SIZE  1024 

#include <iostream>
#include <sstream>
#include <string.h>
#include <boost/json/src.hpp>
#include <boost/json.hpp>
using namespace boost::json;

namespace req_json {

    struct req
    {
        std::string file_name;
        int oper_num;
        int start;
        int end;
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
            value_to<int>(obj.at("start")),
            value_to<int>(obj.at("end"))
        };
    }

};
/*
int main() {





    req_json::req c{ "1.bin", 0 };
    std::string cc = serialize(value_from(c));
    value jv = value_from(cc);
    std::cout << cc << "\n";
    std::cout << jv;
    //req_json::req c1( value_to<req_json::req>(jv) );
    value vv = parse(cc);
    req_json::req cd = value_to<req_json::req>(vv);

    std::cout << vv << "\n";
    std::cout << cd.file_name;
    return 0;

}*/