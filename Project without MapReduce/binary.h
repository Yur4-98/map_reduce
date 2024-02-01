#define _CRT_SECURE_NO_DEPRECATE

#include <ctime>
#include <iostream>
#include <vector>
#include <string>

#define CNT_MAX 100
#define OBS_MAX 10000

typedef struct connection_log
{
    int server_num;
    int connections_count;
} connection_log;

void generate_file(char* file_path, std::vector<int> servers_num_vector);
void test_output(char* file_path, std::vector<int> servers_num_vector);
