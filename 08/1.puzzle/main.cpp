#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

std::vector<std::string> getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return std::vector<std::string>();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string line;
    
    std::vector<std::string> input;
    
    while(std::getline(ss, line, '\n')) {
        size_t pos = line.find('|');
        std::string buffer = line.substr(pos + 2);
        std::stringstream _ss(buffer);
        buffer = "";
        while(std::getline(_ss, buffer, ' ')) {
            input.push_back(buffer);
        }
    }
    return input;
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }

    const size_t SEG_ONE = 2;
    const size_t SEG_SEVEN = 3;
    const size_t SEG_FOUR = 4;
    const size_t SEG_EIGHT = 7;

    std::vector<std::string> segments = getInput(argv[1]);
    size_t count = 0;
    for(const auto& segment : segments) {
        size_t size = segment.size();
        if( size == SEG_ONE ||
            size == SEG_SEVEN ||
            size == SEG_FOUR ||
            size == SEG_EIGHT) {
                count++;
            }
    }

    std::cout << count << std::endl;
    return 0;
}