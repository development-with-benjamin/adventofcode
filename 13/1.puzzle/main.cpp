#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

class Point
{
public:

    Point() = delete;
    Point(const size_t x, const size_t y) : x(x), y(y) {}; 
    size_t x;
    size_t y;

    friend bool operator==(const Point& a, const Point& b);
};

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

struct Instruction
{
    Instruction() = delete;
    Instruction(const char axis, const size_t coordinate) : axis(axis), coordinate(coordinate) {};
    char axis;
    size_t coordinate;
};


class Matrix
{
private:
    std::vector<Point> coordinates;
    std::vector<Instruction> instructions;

    void foldX(const size_t coordinate) {
        for(Point& point : this->coordinates) {
            point.x = point.x > coordinate ? point.x - 2 * (point.x - coordinate) : point.x;
        }
    };
    
    void foldY(const size_t coordinate) {
        for(Point& point : this->coordinates) {
            point.y = point.y > coordinate ? point.y - 2 * (point.y - coordinate) : point.y;
        }
    };

    void fold(const Instruction& instruction) {
        if(instruction.axis == 'x') {
            foldX(instruction.coordinate);
        }
        else {
            foldY(instruction.coordinate);
        }
        eliminateDups();
    };

    void eliminateDups() {
        for(size_t i = 0; i < this->coordinates.size() - 1; ++i) {
            for(size_t j = i + 1; j < this->coordinates.size(); ++j) {
                if(this->coordinates.at(i) == this->coordinates.at(j)) {
                    this->coordinates.erase(this->coordinates.begin() + j);
                    --j;
                }
            }
        }
    };

public:
    Matrix() = default;

    void setInstructions(const std::vector<Instruction>& instructions) { this->instructions = instructions; };
    void setCoordinates(const std::vector<Point>& coordinates) { this->coordinates = coordinates; };
    
    std::vector<Point> getCoordinates() const { return coordinates; };

    void fold(size_t instructionNumber) {
        fold(this->instructions.at(instructionNumber));
    };
    void foldAll() {
        for(Instruction instruction : this->instructions) {
            fold(instruction);
        }
    };

    bool hasPoint(const size_t x, const size_t y) const {
        for(Point point : this->coordinates) {
            if(point.x == x && point.y == y) {
                return true;
            }
        }
        return false;
    }

    size_t maxX() const {
        size_t max = 0; 
        for(Point point : this->coordinates) {
            if(point.x > max) {
                max = point.x;
            }
        }
        return max;
    }
    
    size_t maxY() const {
        size_t max = 0; 
        for(Point point : this->coordinates) {
            if(point.y > max) {
                max = point.x;
            }
        }
        return max;
    }
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};

std::ostream& operator<<(std::ostream& os, const Matrix& m)  {
    size_t maxX = m.maxX();
    size_t maxY = m.maxY();

    for(size_t y = 0; y <= maxY; ++y) {
        for(size_t x = 0; x <= maxX; ++x) {
            if(m.hasPoint(x, y)) {
                os << '#';
            }
            else {
                os << '.';
            }
        }
        os << std::endl;
    }
    return os;
}

Matrix getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return Matrix();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string line;
    
    Matrix input;
    std::vector<Point> coordinates;
    std::vector<Instruction> instructions;
    std::string foldAlong = "fold along ";

    while(std::getline(ss, line, '\n')) {
        size_t pos = line.find(',');
        if(pos == std::string::npos) {
            pos = line.find(foldAlong);
            if(pos == std::string::npos) {
                continue;
            }
            if(line.at(pos + foldAlong.size()) == 'x') {
                instructions.push_back(Instruction('x', stoi(line.substr(pos + foldAlong.size() + 2))));
            } else if(line.at(pos + foldAlong.size()) == 'y') {
                instructions.push_back(Instruction('y', stoi(line.substr(pos + foldAlong.size() + 2))));
            }
        }
        else {
            size_t x = stoi(line.substr(0, pos));
            size_t y = stoi(line.substr(pos + 1));
            coordinates.push_back(Point(x, y));
        }
    }
    input.setCoordinates(coordinates);
    input.setInstructions(instructions);
    return input;
}

int main(const int argc, const char** argv)
{
    if(argc != 2) {
        return -1;
    }

    Matrix matrix = getInput(argv[1]);
    matrix.fold(0);

    std::cout << matrix.getCoordinates().size() << std::endl;

    return 0;
}