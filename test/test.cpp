#include "binary.h"
#include <unordered_map>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <io.h>
//comment
/*
std::unordered_map<long, long> mapping(const char* filename, long left, long right)
{
    std::unordered_map<long, long> mapper_output;
    FILE* F = fopen(filename, "rb");
    fseek(F, left * sizeof(connection_log), SEEK_SET);
    long width = right - left;
    connection_log temp;
    for (long i = 0; i < width; i++)
    {
        fread(&temp, sizeof(connection_log), 1, F);
        //std::cout << "SERVER: " << temp.server_num << '\n';
        //std::cout << "COUNT: " << temp.connections_count << '\n';
        if (mapper_output.find(temp.server_num) == mapper_output.end())
        {
            mapper_output[temp.server_num] = temp.connections_count;
        }
        else
        {
            mapper_output[temp.server_num] += temp.connections_count;
        }
    }
    fclose(F);
    return mapper_output;
}*/

int main() {

    std::unordered_map<long, long> res;
    std::cout << res.size();

}