#include "binary.h"

void generate_file(char* file_path, std::vector<int> servers_num_vector)
{
    FILE* F = fopen(file_path, "wb+");
    srand(time(0));
    connection_log input;
    for (int i = 0; i < OBS_MAX; i++)
    {
        input.server_num = servers_num_vector[std::rand() % servers_num_vector.size()];
        input.connections_count = std::rand() % CNT_MAX;
        //std::cout << "test" << input.server_num << '\n';
        //std::cout << "test" << input.connections_count << '\n';
        fwrite(&input, sizeof(connection_log), 1, F);
    }
    fclose(F);
}

void test_output(char* file_path)
{
    FILE* F = fopen(file_path, "rb+");
    connection_log output;
    for (int i = 0; i < OBS_MAX; i++)
    {
        fread(&output, sizeof(connection_log), 1, F);
        std::cout << "SERVER: " << output.server_num << '\n';
        std::cout << "COUNT: " << output.connections_count << '\n';
        std::cout << '\n';
    }
    fclose(F);
}

size_t count_num(char* file_path)
{
    long nFileLen = 0;
    struct _finddata_t fData;
    int res = _findfirst(file_path, &fData);
    //std::cout << fData.size << '\n';
    if (res) nFileLen += fData.size;
    return (size_t)nFileLen / sizeof(connection_log);
}

std::vector<interval> make_interval(long file_size, long mappers_count)
{
    std::vector<interval> intervals;
    long width = file_size / mappers_count;
    for (int i = 0; i < mappers_count; i++)
    {
        interval temp;
        if (i != mappers_count - 1)
        {
            temp.left = width * i;
            temp.right = temp.left + width;
            intervals.push_back(temp);
        }
        else
        {
            temp.left = width * i;
            temp.right = file_size;
            intervals.push_back(temp);
        }
    }
    return intervals;
}

std::unordered_map<long, long> mapping(char *filename, long left, long right)
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
}