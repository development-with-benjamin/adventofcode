#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <bits/stdc++.h>

struct Octopus
{
    Octopus(int energie) : energie(energie), flashed(false) {}
    int energie;
    bool flashed; 
};


std::vector<std::vector<Octopus>> getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return std::vector<std::vector<Octopus>>();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string line;
    
    std::vector<std::vector<Octopus>> input;
    size_t row_cnt = 0;
    while(std::getline(ss, line, '\n')) {
        input.push_back(std::vector<Octopus>());
        for(char c : line) {
            input.at(row_cnt).push_back(Octopus((int) c - '0'));
        }
        ++row_cnt;
    }
    return input;
}

void octopus_shine(std::vector<std::vector<Octopus>>& octopusies, const size_t row, const size_t col, size_t& flash_count){
    size_t maxrow = octopusies.size();
    size_t maxcol = octopusies.at(0).size();

    if(row >= maxrow || col >= maxcol) {
        return;
    }
    octopusies.at(row).at(col).energie += 1 * !octopusies.at(row).at(col).flashed;
    if(octopusies.at(row).at(col).energie > 9 && !octopusies.at(row).at(col).flashed) {
        flash_count++;
        octopusies.at(row).at(col).flashed = true;
        
        octopus_shine(octopusies, row - 1, col, flash_count);
        octopus_shine(octopusies, row + 1, col, flash_count);
        octopus_shine(octopusies, row, col - 1, flash_count);
        octopus_shine(octopusies, row, col + 1, flash_count);
        octopus_shine(octopusies, row - 1, col + 1, flash_count);
        octopus_shine(octopusies, row - 1, col - 1, flash_count);
        octopus_shine(octopusies, row + 1, col + 1, flash_count);
        octopus_shine(octopusies, row + 1, col - 1, flash_count);

        octopusies.at(row).at(col).energie = 0;
    }
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }

    std::vector<std::vector<Octopus>> octupusies = getInput(argv[1]);
    size_t steps = 0;
    size_t flash_count = 0;

    while(flash_count != 100) {
        flash_count = 0;
        for(size_t row = 0; row < octupusies.size(); ++row) {
            for(size_t col = 0; col < octupusies.at(row).size(); ++col) {
                octupusies.at(row).at(col).energie++;
            }
        }

        for(size_t row = 0; row < octupusies.size(); ++row) {
            for(size_t col = 0; col < octupusies.at(row).size(); ++col) {
                if(octupusies.at(row).at(col).energie > 9) {
                    octopus_shine(octupusies, row, col, flash_count);
                }
            }
        }

        for(size_t row = 0; row < octupusies.size(); ++row) {
            for(size_t col = 0; col < octupusies.at(row).size(); ++col) {
                octupusies.at(row).at(col).flashed = false;
            }
        }
        steps++;
    }

    std::cout << steps << std::endl; 

    return 0;
}