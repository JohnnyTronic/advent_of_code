#include "Vec2.h"
#include "Grid.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <map>

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

    Grid grid(parsedLines);
    grid.PrintGrid();

    long long totalEnergizedCells = grid.FireBeam(Vec2(0, 0), Vec2(1, 0));
       
    std::cout << "PART 1 ANSWER - Total count of energized cells: " << totalEnergizedCells << "\n";
    
    // Part 2 - Given a controllable starting beam, what is the most tiles that can be energized?
    grid.Reset();

    long long maxEnergy = 0;
    // Top row moving down
    Vec2 direction = Vec2(0, 1);
    for (int x = 0; x < grid.width; x++) {
        grid.Reset();
        long long score = grid.FireBeam(Vec2(x, 0), direction);
        if (score > maxEnergy)
            maxEnergy = score;
        grid.PrintEnergizedGrid();
    }

    // Right column moving left
    direction = Vec2(-1, 0);
    for (int y = 0; y < grid.height; y++) {
        grid.Reset();
        long long score = grid.FireBeam(Vec2(grid.width - 1, y), direction);
        if (score > maxEnergy)
            maxEnergy = score;
    }

    // Bottom row moving up
    direction = Vec2(0, -1);
    for (int x = 0; x < grid.width; x++) {
        grid.Reset();
        long long score = grid.FireBeam(Vec2(x, grid.height - 1), direction);
        if (score > maxEnergy)
            maxEnergy = score;
    }

    // Left column moving right
    direction = Vec2(1, 0);
    for (int y = 0; y < grid.height; y++) {
        grid.Reset();
        long long score = grid.FireBeam(Vec2(0, y), direction);
        if (score > maxEnergy)
            maxEnergy = score;
    }
        
    std::cout << "PART 2 ANSWER - Max energized cells: " << maxEnergy << "\n";

    return 0;
}