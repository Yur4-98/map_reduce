#include "binary.h"

int main()
{
    std::vector<int> servers_num_vector = { 1, 2, 3, 4 };
    char file_path[16] = "time_serie.txt";

    generate_file(file_path, servers_num_vector);
    test_output(file_path, servers_num_vector);

    return 0;
}

//void generate_file(char* file_path, std::vector<int> servers_num_vector)
//{
//    FILE* F = fopen(file_path, "wb+");
//    srand(time(0));
//    connection_log input;
//    for (int i = 0; i < OBS_MAX; i++)
//    {
//        input.server_num = servers_num_vector[std::rand() % servers_num_vector.size()];
//        input.connections_count = std::rand() % CNT_MAX;
//        std::cout << "test" << input.server_num << '\n';
//        std::cout << "test" << input.connections_count << '\n';
//        fwrite(&input, sizeof(connection_log), 1, F);
//    }
//    fclose(F);
//}
//
//void test_output(char* file_path, std::vector<int> servers_num_vector)
//{
//    FILE* F = fopen(file_path, "rb+");
//    connection_log output;
//    for (int i = 0; i < OBS_MAX; i++)
//    {
//        fread(&output, sizeof(connection_log), 1, F);
//        std::cout << "SERVER: " << output.server_num << '\n';
//        std::cout << "COUNT: " << output.connections_count << '\n';
//        std::cout << '\n';
//    }
//    fclose(F);
//}