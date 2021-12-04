#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>

class Board
{
public:
    Board() = delete;
    Board(const std::string& str)
    {
        std::string line;
        std::stringstream ss(str);
        while(std::getline(ss, line, '\n')) {
            std::string number;
            std::stringstream linestream(line);
            while(std::getline(linestream, number, ' ')) {
                if(number.empty()) {
                    continue;
                }
                board.push_back(std::stoi(number));
            }
            width++;
        }
    };

    bool checkBingo(int draw) {
        bool found = false;
        for(auto& number : board) {
            if(number == draw) {
                number = -1;
                found = true;
            }
        }
        if(!found) {
            return false;
        }

        // check horizontally
        int checkflag = 0;
        for(int i = 0; i < board.size(); ++i) {
            if(board.at(i) != -1) {
                i = (i/width) * width + width;
                i--;
                checkflag = 0;
            } else if(++checkflag == width){
                return true;
            }
        }

        // check vertically
        checkflag = 0;
        for(int i = 0; i < board.size(); i += width) {
            if(board.at(i) != -1) {
                i = ((i+1) % width) - width;
                checkflag = 0;
                if(i == -width) {
                    break;
                }
            } else if(++checkflag == width){
                return true;
            }
        }
        return false;
    };

    int getBoardSum() {
        int sum = 0;
        for (auto i : board) {
            if(i != -1) {
                sum += i;
            }
        }
        return sum;
    }


private:
    int width = 0;
    std::vector<int> board;
};

std::vector<int> convertStringRepresantingNumbers2AContainerHoldingIntegers(std::string& stringRepresantingNumbers) {
    std::stringstream ss(stringRepresantingNumbers);
    std::string number;
    std::vector<int> containerHoldingIntegers;
    while(std::getline(ss, number, ',')){
        containerHoldingIntegers.push_back(stoi(number));
    }
    return containerHoldingIntegers;
}

std::pair<std::vector<int>, std::vector<Board>> get_elements(const char* filename)
{
    std::ifstream ifs(filename);
    if(!ifs.is_open()){
        return {std::vector<int>(), std::vector<Board>()};
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    
    std::string line;
    std::getline(ss, line, '\n');
    std::vector<int> draws = convertStringRepresantingNumbers2AContainerHoldingIntegers(line);
    std::string board;
    std::vector<Board> boards;

    while(std::getline(ss, line, '\n')) {
        if(line.empty() && !board.empty()) {
            boards.push_back(Board(board));
            board = "";
            continue;
        } else if(!line.empty()) {
            board += line + "\n";
        }
    }
    return {draws, boards};
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }

    auto DrawsAndBoards = get_elements(argv[1]);
    for(auto draw : DrawsAndBoards.first) {
        
        for(int i = 0; i < DrawsAndBoards.second.size(); ++i) {
            if(DrawsAndBoards.second.at(i).checkBingo(draw)) {
                std::cout << DrawsAndBoards.second.at(i).getBoardSum() * draw << std::endl;
                return 0; 
            }
        }

    }
    return 0;
}