#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

#define PARANTHESIS 1
#define BRAKETS 2
#define CURLY_BRAKETS 3
#define POINTY_BRAKETS 4

class Braket
{
public:
    Braket() = delete;
    Braket(char open, char close, int value) : open(open), close(close), value(value) {};

    char getOpen() const { return open; };
    char getClose() const { return close; };
    int getValue() const { return value; };
private:
    char open;
    char close;
    int value;
};

bool getSyntaxError(const std::string& line, const std::vector<Braket>& brakets, std::vector<Braket>& stack)
{
    for(const char c : line) {
        for(const auto& braket : brakets) {
           if( c == braket.getOpen()) {
               stack.push_back(braket);
           }
           else if( c == braket.getClose()) {
               if(c == stack.back().getClose()) {
                   stack.pop_back();
               }
               else {
                   return true;
               }
           }
        }   
    }
    return false;
}

std::vector<std::string> getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return std::vector<std::string>();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string line;
    
    std::vector<std::string> input;
    while(std::getline(ss, line, '\n')) {
        input.push_back(line);        
    }
    return input;
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }
    std::vector<std::string> syntax = getInput(argv[1]);

    std::vector<size_t> syntax_error_score;
    std::vector<Braket> brakets{Braket('(', ')', PARANTHESIS), Braket('[', ']', BRAKETS), Braket('{', '}', CURLY_BRAKETS), Braket('<', '>', POINTY_BRAKETS) };

    for(std::string& line : syntax) {
        std::vector<Braket> stack;
        if(!getSyntaxError(line, brakets, stack)) {
            if(stack.size() == 0) {
                continue;
            }
            size_t error_score = 0;
            for(int i = stack.size() - 1; i >= 0; --i) {
                error_score *= 5;
                error_score += stack.at(i).getValue();
            }
            syntax_error_score.push_back(error_score);
        }
    }
    std::sort(syntax_error_score.begin(), syntax_error_score.end());
    size_t middle_index = (syntax_error_score.size()/2);
    std::cout << syntax_error_score.at(middle_index) << std::endl;
    return 0;
}