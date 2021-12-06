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
    
    std::vector<size_t> fish(9, 0);

    while(std::getline(ss, number, ',')) {
        fish.at(stoi(number))++;
    }
    return fish;
}

void swap(size_t* a, size_t* b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }
    const int DAYS = 80;
    std::vector<size_t> fish = getInput(argv[1]);
    for(int i = 0; i < DAYS; ++i) {
        for(int j = 1; j < fish.size(); ++j) {
            swap(&fish.at(j - 1), &fish.at(j));
        }
        fish.at(6) += fish.at(8);
    }
    size_t sum = 0;
    for(int i = 0; i < fish.size(); ++i) {
        sum += fish.at(i);
    }
    std::cout << sum << std::endl;
    return 0;
}
