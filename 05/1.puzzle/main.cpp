#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

struct Point {
    size_t x;
    size_t y;
};

struct Line {
    Point p1;
    Point p2;
};

class Field {
public:
    Field() = default;

    void drawLine(const Line& line) {
        if(line.p1.x != line.p2.x && line.p1.y != line.p2.y) return;
        adaptLineSize(line);

        int minX = line.p1.x > line.p2.x ? line.p2.x : line.p1.x;
        int maxX = line.p1.x > line.p2.x ? line.p1.x : line.p2.x; 
        int minY = line.p1.y > line.p2.y ? line.p2.y : line.p1.y;
        int maxY = line.p1.y > line.p2.y ? line.p1.y : line.p2.y;
        
        for(int i = minX; i <= maxX; ++i) {
            for(int j = minY; j <= maxY; ++j) {
                field.at(i).at(j)++;
            }
        }
    };

    size_t dangerousAreasCount() {
        size_t count = 0;
        for(const auto& line : field) {
            for(const auto& elem : line) {
                if(elem > 1) {
                    count++;
                }
            }   
        }

        return count;
    }


private:
    std::vector<std::vector<size_t>> field;

    void adaptLineSize(const Line& line) {
        resizeField(line.p1.x+1);
        resizeField(line.p2.x+1);
        resizeField(line.p1.y+1);
        resizeField(line.p2.y+1);
    }

    void resizeField(size_t size) {
        if(size > field.size()) {
            field.resize(size);
            for(auto& v : field) {
                v.resize(size);
            }
        }
    }
};

std::vector<Line> readLines(const char* filename) {

    std::ifstream ifs(filename);
    if(!ifs.is_open()){
        return std::vector<Line>();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    
    std::string line;

    std::vector<Line> lines;
    while(std::getline(ss, line, '\n')) {
        Line l;
        size_t pos = line.find(" -> ");
        std::string p1Str = line.substr(0, pos);
        std::string p2Str = line.substr(pos + 4);

        pos = p1Str.find(",");
        l.p1.x = stoi(p1Str.substr(0, pos));
        l.p1.y = stoi(p1Str.substr(pos+1));
        pos = p2Str.find(",");
        l.p2.x = stoi(p2Str.substr(0, pos));
        l.p2.y = stoi(p2Str.substr(pos+1));
        lines.push_back(l);
    }

    return lines;
}

int main(int argc, const char* argv[]) {

    if (argc != 2) {
        return -1;
    }

    Field field;
    std::vector<Line> lines = readLines(argv[1]);

    for(const auto& line : lines) {
        field.drawLine(line);
    }

    std::cout << field.dangerousAreasCount() << std:: endl;

    return 0;
}

