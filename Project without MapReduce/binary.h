#define _CRT_SECURE_NO_DEPRECATE

#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <io.h>
#include <unordered_map>

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

void generate_file(char* file_path, std::vector<int> servers_num_vector);
void test_output(char* file_path);
size_t count_num(char* file_path);
std::vector<interval> make_interval(long file_size, long mappers_count);
std::unordered_map<long, long> mapping(char* filename, long left, long right);
void map_unification(std::unordered_map<long, long>* unified_map, std::unordered_map<long, long> imported_map);
