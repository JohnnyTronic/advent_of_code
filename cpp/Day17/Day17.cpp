#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <map>

class Grid {
public:
    
};
int main()
{
    std::cout << "Advent of Code - Day 16!\n";

    std::ifstream ifs("input.txt");
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    //int[3] myArray();
    //Grid grid(parsedLines);
}
