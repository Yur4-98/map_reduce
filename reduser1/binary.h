#define _CRT_SECURE_NO_DEPRECATE

#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <io.h>
#include <unordered_map>
#include <boost/json/src.hpp>
#include <boost/json.hpp>
using namespace boost::json;

#define CNT_MAX 100
#define OBS_MAX 14888
#define MAP_COUNT 4


typedef struct connection_log
{
    int server_num;
    int connections_count;
} connection_log;

typedef struct interval
{
    long left;
    long right;
} interval;

void tag_invoke(value_from_tag, value& jv, connection_log const& c)
{
    jv = {
        { "server_num" , c.server_num },
        { "connections_count", c.connections_count }
    };
}

v tag_invoke(value_to_tag< connection_log >, value const& jv)
{
    object const& obj = jv.as_object();
    return connection_log{
        value_to<int>(obj.at("server_num")),
        value_to<int>(obj.at("connections_count"))
    };
}


void generate_file(char* file_path, std::vector<int> servers_num_vector);
void test_output(char* file_path);
size_t count_num(const char* file_path);
std::vector<interval> make_interval(long file_size, long mappers_count);
std::unordered_map<long, long> mapping(const char* filename, long left, long right);
void map_unification(std::unordered_map<long, long>* unified_map, std::unordered_map<long, long> imported_map);
connection_log find_max(std::unordered_map<long, long> returned_map);