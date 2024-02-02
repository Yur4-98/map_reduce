#include "binary.h"

int main()
{
    std::vector<int> servers_num_vector = { 1, 2, 3, 4};
    char file_path[16] = "time_serie.txt";

    generate_file(file_path, servers_num_vector);
    //test_output(file_path);
    long size = count_num(file_path);
    std::cout << "size " << size << '\n';
    std::vector<interval> test = make_interval(size, MAP_COUNT);
    for (int i = 0; i < MAP_COUNT; i++)
    {
        std::cout << "( " << test[i].left << "; " << test[i].right << ")\n";
    }
    return 0;
}