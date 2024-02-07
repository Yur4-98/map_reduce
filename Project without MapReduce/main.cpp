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
    std::unordered_map <long, long> test_map;
    connection_log temp;
    for (int i = 0; i < MAP_COUNT; i++)
    {
        std::cout << "( " << test[i].left << "; " << test[i].right << ")\n";
        test_map = mapping(file_path, test[i].left, test[i].right);
        std::cout << test_map[1] << "\n";
        std::cout << test_map[2] << "\n";
        std::cout << test_map[3] << "\n";
        std::cout << test_map[4] << "\n";
        temp = find_max(test_map);
        std::cout << "server: " << temp.server_num << " counts: " << temp.connections_count << "\n";

    }
    
    return 0;
}