#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> get_elements(const char* elements)
{
    std::stringstream ss;
    ss << elements;

    std::string elem;
    std::vector<std::string> elem_con;

    while(std::getline(ss, elem, '\n')) {
        elem_con.push_back(elem);
    }
    return elem_con;
}

int main(const int argc, const char** argv)
{
    if(argc != 2){
        return -1;
    }

    std::vector<std::string> diagnostics = get_elements(argv[1]);
    size_t gamma = 0;
    size_t epsilon = 0;

    for(int i = 0; i < diagnostics[0].size(); ++i) {
        size_t count_ones = 0;
        for(const auto& entry : diagnostics) {
            if(entry[i] == '1') {
                count_ones++;
            }
        }
        gamma += 1*(count_ones > diagnostics.size()/2);
        gamma = gamma << 1;
    }
    // 111011000100
    gamma = gamma >> 1;
    epsilon = ~gamma & 0xfff;
    std::cout << gamma * epsilon << std::endl;
    return 0;
}