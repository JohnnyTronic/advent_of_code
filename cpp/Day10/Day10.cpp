#include "Grid.cpp"
#include "Vec2.h"
#include "Direction.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <set>
#include <regex>
#include <map>

std::vector<Vec2> GetPipeNeighbourOffsets(char& pipeType) {
    std::vector<Vec2> neighbours;
    switch (pipeType) {
    case '|': return std::vector<Vec2>{Vec2(0, -1), Vec2(0, 1) };
    case '-': return std::vector<Vec2>{Vec2(-1, 0), Vec2(1, 0)};
    case 'L': return std::vector<Vec2>{Vec2(0, -1), Vec2(1, 0)};
    case 'J': return std::vector<Vec2>{Vec2(-1, 0), Vec2(0, -1)};
    case '7': return std::vector<Vec2>{Vec2(-1, 0), Vec2(0, 1)};
    case 'F': return std::vector<Vec2>{Vec2(1, 0), Vec2(0, 1)};
    case '.': return std::vector<Vec2>{};
    case 'S': return std::vector<Vec2>{};
    default: return std::vector<Vec2>{};
    }
}

//std::vector<Vec2> FindNeighboursForStartingCell(const Vec2& startingCell, Grid& grid) {
//}

int main()
{
    std::cout << "Advent of Code - Day 10!\n";
    
    std::ifstream ifs("test_input.txt");
    //std::ifstream ifs("input.txt");

    // Parse each line/history
    std::vector<std::string> parsedLines;
    std::string parsedLine;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    Grid grid(parsedLines);
    
    auto targetCell = Vec2(2, 4);
    auto zerozero = grid.GetCharAt(targetCell);
    auto neighbourOffsets = GetPipeNeighbourOffsets(zerozero);    
    std::cout << "Neighbours of " << targetCell << " are ";
    for (auto neighbour : neighbourOffsets) {
        std::cout << targetCell + neighbour << ", ";
    }
        
    return 0;
}