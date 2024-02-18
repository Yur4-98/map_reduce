#define _CRT_SECURE_NO_DEPRECATE

#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <io.h>
#include <unordered_map>

#define CNT_MAX 100
#define OBS_MAX 15
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

typedef struct stat_log
{
    long sum_counts;
    long sum_connections;
} stat_log;

void generate_file(char* file_path, std::vector<int> servers_num_vector);
void test_output(char* file_path);
size_t count_num(const char* file_path);
std::vector<interval> make_interval(long file_size, long mappers_count);
std::unordered_map<long, stat_log> mapping(const char* filename, long left, long right);
void map_unification(std::unordered_map<long, stat_log>* unified_map, std::unordered_map<long, stat_log> imported_map);
//std::pair <long, stat_log> find_max(std::unordered_map<long, stat_log> returned_map);