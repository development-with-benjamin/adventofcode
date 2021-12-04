#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <sstream>
#include <vector>

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

#endif // BOARD_H