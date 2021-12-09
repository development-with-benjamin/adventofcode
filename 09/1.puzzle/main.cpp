#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

struct Point
{
    Point() = delete;
    Point(int x, int y) : x(x), y(y) {};
    int x;
    int y;
};


std::vector<std::vector<size_t>> getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return std::vector<std::vector<size_t>>();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string line;
    
    std::vector<std::vector<size_t>> input;
    size_t idx_cnt = 0;
    while(std::getline(ss, line, '\n')) {
        input.push_back(std::vector<size_t>());
        for(char c : line) {
            input.at(idx_cnt).push_back((size_t) (c - '0'));
        }
        idx_cnt++;
    }
    return input;
}

size_t check_risk(const std::vector<std::vector<size_t>>& heatmap, const Point& p)
{
    size_t maxrow = heatmap.size();
    size_t maxcol = heatmap.at(p.x).size();

    std::vector<Point> mask {Point(p.x - 1, p.y), Point(p.x + 1, p.y), Point(p.x, p.y - 1), Point(p.x, p.y + 1)};

    for(Point& point : mask) {
        if(point.x >= maxcol || point.x < 0) {
            continue;
        }
        if(point.y >= maxrow || point.y < 0) {
            continue;
        }
        if(heatmap.at(point.x).at(point.y) <= heatmap.at(p.x).at(p.y)) {
            return 0;
        }
    }
    return heatmap.at(p.x).at(p.y) + 1;
}

int main(int argc, char** argv) 
{
    if(argc != 2) {
        return -1;
    }

    std::vector<std::vector<size_t>> heatmap = getInput(argv[1]);
    size_t risk_level = 0;

    for(size_t i = 0; i < heatmap.size(); ++i) {
        for(size_t j = 0; j < heatmap.at(i).size(); ++j) {
            risk_level += check_risk(heatmap, Point(i, j));           
        }
    }

    std::cout << risk_level << std::endl;
    return 0;
}