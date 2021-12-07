#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

std::vector<size_t> getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return std::vector<size_t>();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string number;
    
    std::vector<size_t> input;
    size_t biggest = 0;
    
    while(std::getline(ss, number, ',')) {
        size_t n = stoi(number);
        if(n > biggest) {
            biggest = n;
            input.resize(biggest + 1);
        }
        input.at(n)++;
    }
    return input;
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }

    std::vector<size_t> crabs = getInput(argv[1]);

    size_t minFuel = (~ (size_t)0);
    for(int i = 0; i < crabs.size(); ++i) {
        size_t fuel = 0;
        for(int j = 0; j < crabs.size(); ++j) {
            fuel += (crabs.at(j) * abs(j - i)); 
        }
        if(fuel < minFuel) {
            minFuel = fuel;
        }
    }

    std::cout << minFuel << std::endl;

    return 0;
}