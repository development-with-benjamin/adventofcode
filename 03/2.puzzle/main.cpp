#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> get_elements(const char* elements)
{
    std::stringstream ss;
    ss << elements;

    std::string elem;
    std::vector<std::string> elem_con;

    while(std::getline(ss, elem, '\n')) {
        elem_con.push_back(elem);
    }
    return elem_con;
}

std::string searchCommon(std::vector<std::string> diagnostics, const bool least_common, size_t recursion_count=0)
{
    if(diagnostics.size() <= 1){
        return diagnostics.at(0);
    }
    size_t count_ones = 0;
    for(const auto& entry : diagnostics) {
        if(entry[recursion_count] == '1') {
            count_ones++;
        }
    }
    char common = 0;
    if(least_common) {
        common = (char) (1*((float)count_ones < (float)diagnostics.size()/2.));
    }else {
        common = (char) (1*((float)count_ones >= (float)diagnostics.size()/2.));
    }
    common += 48;
    for(int i = 0; i < diagnostics.size(); ++i) {
        if(diagnostics.at(i)[recursion_count] != common) {
            diagnostics.erase(diagnostics.begin() + i);
            --i;
        }
    }
    return searchCommon(diagnostics, least_common, ++recursion_count);
}

size_t convertBinary(const std::string& binary)
{
    size_t result = 0;
    for(char c : binary){
        if(c == '1') {
            result += 1;
        }
        result = result << 1;
    }
    return result >> 1;
}

int main(const int argc, const char** argv)
{
    if(argc != 2){
        return -1;
    }

    std::vector<std::string> diagnostics = get_elements(argv[1]);
    size_t o2 = convertBinary(searchCommon(diagnostics, false));
    size_t co2 = convertBinary(searchCommon(diagnostics, true));

    std::cout << o2 * co2 << std::endl; 
    return 0;
}