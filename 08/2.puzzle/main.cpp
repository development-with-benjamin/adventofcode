#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

class Segment {
public: 
    Segment() = delete;
    Segment(const std::string& line) {
        size_t posPipe = line.find('|');
        std::string wireInput = line.substr(0, posPipe);
        std::string segmentOutput = line.substr(posPipe + 2);

        std::stringstream sswi(wireInput);
        std::stringstream ssso(segmentOutput);
        std::string buffer;
        while(std::getline(sswi, buffer, ' ')) {
            this->m_wireInput.push_back(buffer);
        }
        while(std::getline(ssso, buffer, ' ')) {
            std::sort(buffer.begin(), buffer.end());
            this->m_segmentOutput.push_back(buffer);
        }
        this->segments.resize(10);
        this->deduceSegments();
    }

    size_t getValue() { return value; };

private:
    std::vector<std::string> m_wireInput;
    std::vector<std::string> m_segmentOutput;

    size_t value = 0;

    void deduceSegments() {
        deduceInput();
        for(size_t i = 0; i < this->m_segmentOutput.size(); ++i) {
            for(size_t j = 0; j < this->segments.size(); ++j) {
                if(this->m_segmentOutput.at(i).compare(this->segments.at(j)) == 0) {
                    size_t exponent = this->m_segmentOutput.size() - 1 - i;
                    this->value += (j * (size_t) std::pow(10, exponent));
                }
            }
        }
    };

    void deduceInput() {
        for(const auto& input : this->m_wireInput) {
            size_t inputsize = input.size();
            if (inputsize == this->SEG_ONE) {
                this->segments[1] = input;
            }
            else if (inputsize == this->SEG_SEVEN) {
                this->segments[7] = input;
            }
            else if (inputsize == this->SEG_FOUR){
                this->segments[4] = input;
            }
            else if (inputsize == this-> SEG_EIGHT) {
                this->segments[8] = input;
            }
        }
        this->A = findUniqueChar(this->segments[7], this->segments[1]);
        this->G = matchAllButOne(this->segments[4] + A);
        this->D = matchAllButOne(this->segments[1] + A + G);
        
        for(const auto& input : this->m_wireInput) {
            if(input.size() == 6) {
                if(!does1match2(this->segments[4] + this->A + this->G, input)) {
                    if(input.find(D) != std::string::npos) {
                        this->segments.at(6) = input;
                        this->F = findCommonChar(input, this->segments[1]);
                        std::stringstream ss;
                        ss << this->F;            
                        this->C = findUniqueChar(this->segments[1], ss.str());
                    }
                    else {
                        this->segments.at(0) = input;
                    }
                }
                else if(does1match2(this->segments[4] + this->A + this->G, input)) {
                    this->segments[9] = input;
                }
            }
        }

        for(const auto& input : this->m_wireInput) {
            if(input.size() == 5) {
                std::stringstream ss;
                ss << this->A << this->D << this->G << this->F;
                if(does1match2(ss.str(), input)) {
                    std::stringstream ssc;
                    ssc << this->C;
                    if(!does1match2(ssc.str(), input)) {
                        std::stringstream adgf;
                        adgf << this->A << this->D << this->G << this->F;
                        this->B = findUniqueChar(input, adgf.str());
                    }
                }
            }
        }

        std::stringstream abcdfg;
        abcdfg << this->A << this->B << this->C << this->D << this->F << this->G;
        this->E = findUniqueChar(abcdfg.str(), this->segments[8]);

        std::stringstream ss;
        ss << this->A << this->C << this->D << this->E  << this->G;
        this->segments[2] = ss.str();

        ss.str("");
        ss << this->A << this->C << this->D << this->F << this->G;
        this->segments[3] = ss.str();

        ss.str("");
        ss << this->A << this->B << this->D << this->F << this->G;
        this->segments[5] = ss.str();
        
        for(auto& segment : this->segments) {
            std::sort(segment.begin(), segment.end());
        }
    };

    char findUniqueChar(const std::string& str1, const std::string& str2) {
        int charValue = 0;
        for(const auto c : str1) {
            charValue += c;
        }
        for(const auto c : str2) {
            charValue -= c;
        }
        return (char) abs(charValue);
    };

    char findCommonChar(const std::string& str1, const std::string& str2) {
        char common;
        for(size_t i = 0; i < str1.size(); ++i) {
            for(size_t j = 0; j < str2.size(); ++j) {
                if(str1[i] == str2[j]) {
                    common = str1[i];
                    return common;
                }
            }
        }
        return 0;
    };

    char matchAllButOne(const std::string& string) {
        for(const auto& input : this->m_wireInput) {
            if(input.size() == string.size() + 1) {
                if(does1match2(string, input)) {
                    return findUniqueChar(string, input);
                }
            }
        }
        return 0;
    };

    bool does1match2(const std::string str1, std::string str2) {
        for(size_t i = 0; i < str1.size(); ++i) {
            if(str2.find(str1[i]) == std::string::npos) {
                return false;
            }
        }
        return true;
    };

    const size_t SEG_ONE = 2;
    const size_t SEG_SEVEN = 3;
    const size_t SEG_FOUR = 4;
    const size_t SEG_EIGHT = 7;

    char A;
    char B;
    char C;
    char D;
    char E;
    char F;
    char G;
    
    std::vector<std::string> segments;
};

std::vector<Segment> getInput(const char* filename) {
    std::fstream ifs(filename);
    if(!ifs.is_open()) {
        return std::vector<Segment>();
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string line;
    
    std::vector<Segment> input;
    
    while(std::getline(ss, line, '\n')) {
        input.push_back(Segment(line));
    }
    return input;
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        return -1;
    }

    std::vector<Segment> segments = getInput(argv[1]);
    size_t sum = 0;

    for(auto& s : segments) {
        sum += s.getValue();
    }

    std::cout << sum << std::endl;

    return 0;
}