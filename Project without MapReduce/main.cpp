#include "binary.h"

int main()
{
    std::vector<int> servers_num_vector = { 1, 2, 3, 4};
    char file_path[16] = "time_serie.txt";

    generate_file(file_path, servers_num_vector);
    //test_output(file_path);
    std::cout << "size " << count_num(file_path) << '\n';

    return 0;
}