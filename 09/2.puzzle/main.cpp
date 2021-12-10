#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

struct Point
{
    Point() = delete;
    Point(int x, int y) : x(x), y(y) {};
    int x;
    int y;
};

size_t getHeatmapValue(const std::vector<std::vector<size_t>>& h, const Point& p)
{
    return h.at(p.x).at(p.y);
}

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

bool isMinima(const std::vector<std::vector<size_t>>& heatmap, const Point& p)
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
            return false;
        }
    }
    return true;
}

bool uniqueAdd(std::vector<Point>& vec, const Point p) {
    for(auto& point : vec) {
        if(point.x == p.x && point.y == p.y) {
            return false;
        }
    }
    vec.push_back(p);
    return true;
}

void getBasin(const std::vector<std::vector<size_t>>& heatmap, std::vector<Point>& vec, Point point) {
    
    if(point.x < 0 || point.x >= heatmap.size() || 
        point.y < 0 || point.y >= heatmap.at(point.x).size()) {
            return;
    }

    if(getHeatmapValue(heatmap, point) == 9) {
        return;
    }

    if(!uniqueAdd(vec, point)) {
        return;
    }

    getBasin(heatmap, vec, Point(point.x - 1, point.y));
    getBasin(heatmap, vec, Point(point.x + 1, point.y));
    getBasin(heatmap, vec, Point(point.x, point.y - 1));
    getBasin(heatmap, vec, Point(point.x, point.y + 1));
}

int main(int argc, char** argv) 
{
    if(argc != 2) {
        return -1;
    }

    std::vector<std::vector<size_t>> heatmap = getInput(argv[1]);
    std::vector<std::vector<Point>> basins;

    for(size_t col = 0; col < heatmap.size(); ++col) {
        for(size_t row = 0; row < heatmap.at(col).size(); ++row) {
            Point point(col, row);
            if(isMinima(heatmap, point)) {
                std::vector<Point> basin;
                getBasin(heatmap, basin, point);
                basins.push_back(basin);
            }           
        }
    }
    std::sort(basins.begin(), basins.end(), [](std::vector<Point>& v1, std::vector<Point>& v2){
        return v1.size() > v2.size();
    });

    std::cout << basins.at(0).size() * basins.at(1).size() * basins.at(2).size() << std::endl;
    return 0;
}