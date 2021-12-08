#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

struct Point {
    Point() = default;
    Point(size_t x, size_t y): x(x), y(y) {
    };
    
    size_t x;
    size_t y;
};

class Line {
public:
    Line() = default;

    size_t intersectCount(const Line& other) {
        // paralell
        double s1 = slope(other);
        double s2 = slope(*this);

        if(s1 == s2) {
            int count = 0;

            if(s1 == 0) {
                // vertical overlap
                if(this->p1.x == this->p2.x && this->p1.x == other.p1.x) {

                }
                // horizontal overlap
                else if(this->p1.y == this->p2.y && this->p1.y == other.p1.y) {
                    return abs(this->p1.x)
                }
            }
            int distX = abs(this->p1.x - this->p2.x);
            int distY = abs(this->p1.y - this->p2.y);

            for(int i = 0; i < distX; i++) {

                for(int j = 0; j)
            }
        }

        Point p = calcIntersection(other);
        if(p1.x <= p.x && p.x <= p2.x && p1.y <= p.y && p.y <= p2.y) return 1;

        return 0;
        
    };

    double slope(const Line& other) {
        if(other.p1.y == other.p2.y) return 0;
        return (double) (other.p1.x - other.p2.x) / (double) (other.p1.y - other.p2.y);
    }

    Point calcIntersection(const Line& other) {
        double a1 = p2.y - p1.y;
        double b1 = p1.x - p2.x;
        double c1 = a1*(p1.x) + b1*(p1.y);

        // Line CD represented as a2x + b2y = c2
        double a2 = other.p2.y - other.p1.y;
        double b2 = other.p1.x - other.p2.x;
        double c2 = a2*(other.p1.x)+ b2*(other.p1.y);

        double determinant = a1*b2 - a2*b1;

        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;

        return Point(x, y);
    };

    Point p1;
    Point p2;

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

    std::vector<Line> lines = readLines(argv[1]);

    return 0;
}