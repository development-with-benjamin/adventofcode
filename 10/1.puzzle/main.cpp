#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

#define PARANTHESIS 3
#define BRAKETS 57
#define CURLY_BRAKETS 1197
#define POINTY_BRAKETS 25137

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

int getSyntaxError(const std::string& line, const std::vector<Braket>& brakets)
{
    std::vector<char> stack;
    for(const char c : line) {
        for(const auto& braket : brakets) {
           if( c == braket.getOpen()) {
               stack.push_back(braket.getClose());
           }
           else if( c == braket.getClose()) {
               if(c == stack.back()) {
                   stack.pop_back();
               }
               else {
                   return braket.getValue();
               }
           }
        }   
    }
    return 0;
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

    int syntax_error_score = 0;
    std::vector<Braket> brakets{Braket('(', ')', PARANTHESIS), Braket('[', ']', BRAKETS), Braket('{', '}', CURLY_BRAKETS), Braket('<', '>', POINTY_BRAKETS) };

    for(std::string& line : syntax) {
        syntax_error_score += getSyntaxError(line, brakets);
    }
    
    std::cout << syntax_error_score << std::endl;
    return 0;
}