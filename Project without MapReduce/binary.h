#define _CRT_SECURE_NO_DEPRECATE

#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <io.h>

#define CNT_MAX 100
#define OBS_MAX 110000
#define MAP_COUNT 4


typedef struct connection_log
{
    int server_num;
    int connections_count;
} connection_log;

typedef struct interval
{
    int left;
    int right;
} interval;

void generate_file(char* file_path, std::vector<int> servers_num_vector);
void test_output(char* file_path);
long count_num(char* file_path);
std::vector<interval> make_interval(long file_size, long mappers_count);
