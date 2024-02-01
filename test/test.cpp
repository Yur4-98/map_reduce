
#define  BOOST_JSON_STACK_BUFFER_SIZE  1024 

#include <iostream>
#include <sstream>
#include <string.h>
#include <boost/json/src.hpp>
#include <boost/json.hpp>
using namespace boost::json;




/*

namespace req_json {

    struct req
    {
        std::string file_name;
        int oper_num;
    };

    void tag_invoke(value_from_tag, value& jv, req const& c)
    {
        jv = {
            { "file_name" , c.file_name },
            { "oper_num", c.oper_num }
        };
    }

    req tag_invoke(value_to_tag< req >, value const& jv)
    {
        object const& obj = jv.as_object();
        return req{
            value_to<std::string>(obj.at("file_name")),
            value_to<int>(obj.at("oper_num"))
        };
    }

};

int main(){


    req_json::req c{ "1.bin", 0 };
    std::string cc = serialize(value_from(c));
    value jv = value_from(cc);
    std::cout << cc << "\n";
    std::cout << jv;
    //req_json::req c1( value_to<req_json::req>(jv) );

    std::cout << cc<<"\n";
    std::cout << jv;
    return 0;
}
*/



namespace my_app {

    struct customer
    {
        int id;
        std::string name;
        bool current;
    };
    void tag_invoke(value_from_tag, value& jv, customer const& c)
    {
        jv = {
            { "id" , c.id },
            { "name", c.name },
            { "current", c.current }
        };
    }
    customer tag_invoke(value_to_tag< customer >, value const& jv)
    {
        object const& obj = jv.as_object();
        return customer{
            value_to<int>(obj.at("id")),
            value_to<std::string>(obj.at("name")),
            value_to<bool>(obj.at("current"))
        };
    }
}

int main() {


    my_app::customer c{ 1001, "Boost", true };
    std::string cc = serialize(value_from(c));
    value jv = value_from(cc);
    std::cout << cc << "\n";
    std::cout << jv;
    my_app::customer c = parse(cc);

    return 0;
}