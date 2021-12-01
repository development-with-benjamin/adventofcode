#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }

    std::stringstream ss;
    ss << argv[1];

    std::string depth;
    std::getline(ss, depth, '\n');
    size_t last_depth = std::stoi(depth);
    size_t increase_count = 0;

    while(std::getline(ss, depth, '\n')) {
        size_t current_depth = std::stoi(depth);
        if(current_depth > last_depth) {
            increase_count++;
        }
        last_depth = current_depth;
    }

    std::cout << increase_count << std::endl;
}