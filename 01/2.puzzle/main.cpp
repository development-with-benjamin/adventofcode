#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<size_t> get_depths(const char* depths)
{
    std::stringstream ss;
    ss << depths;

    std::string depth;
    std::vector<size_t> depth_cont;

    while(std::getline(ss, depth, '\n')) {
        depth_cont.push_back((size_t) std::stoi(depth));
    }
    return depth_cont;
}

std::vector<size_t> create_moving_sum(const std::vector<size_t>& depths, const size_t window_size)
{
    std::vector<size_t> moving;
    for(size_t i = 0; i + window_size < depths.size(); ++i) {
        size_t sum = 0;
        for(size_t j = 0; j < window_size; ++j) {
            sum += depths.at(i + j);
        }
        moving.push_back(sum);
    }
    return moving;
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }

    std::vector<size_t> depths = create_moving_sum(get_depths(argv[1]), 3);
    size_t increase_count = 0;
    size_t current_depth = depths.at(0);
    for (const auto depth : depths) {
        if(depth > current_depth) {
            increase_count++;
        }
        current_depth = depth;
    }

    std::cout << increase_count << std::endl;
}