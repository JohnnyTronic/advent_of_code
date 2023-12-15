#include "Grid.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

void ExpandSpace(Grid& grid, long long expansionAmount) {
    // Expand empty columns
    for (int x = 0; x < grid.GetWidth(); x++) {
        auto& column = grid.columns[x];
        bool emptySpace = true;
        for (auto galaxy : grid.galaxies) {
            if (galaxy->position.x == x) {
                emptySpace = false;
                break;
            }
        }
        if (emptySpace) {
            std::cout << "Found empty column: " << x << "\n";
            column.expandedSize *= expansionAmount;           
        }
    }

    // Expand empty rows
    for (int y = 0; y < grid.GetHeight(); y++) {
        auto& row = grid.rows[y];
        bool emptySpace = true;
        for (auto galaxy : grid.galaxies) {
            if (galaxy->position.y == y) {
                emptySpace = false;
                break;
            }
        }
        if (emptySpace) {
            std::cout << "Found empty row: " << y << "\n";
            row.expandedSize *= expansionAmount;            
        }
    }
}

Vec2 CalculateGalaxyExpandedPosition(Cell* galaxy, Grid& grid) {
    long long expandedXPos{};
    for (int x = 0; x < galaxy->position.x; x++) {
        expandedXPos += grid.columns[x].expandedSize;
    }

    long long expandedYPos{};
    for (int y = 0; y < galaxy->position.y; y++) {
        expandedYPos += grid.rows[y].expandedSize;
    }

    return Vec2(expandedXPos, expandedYPos);
}


int main()
{
    std::cout << "Advent of Code - Day 11!\n";

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    std::vector<std::string> parsedLines;
    std::string parsedLine;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    Grid grid(parsedLines);

    std::cout << "Galaxy0 pos BEFORE expansion: " << CalculateGalaxyExpandedPosition(grid.galaxies[0], grid) << "\n";
           
    ExpandSpace(grid,1);
    
    std::cout << "Galaxy0 pos AFTER expansion: " << CalculateGalaxyExpandedPosition(grid.galaxies[0], grid) << "\n";   
   
    long long totalDistance = 0;
    for (int i = 0; i < grid.galaxies.size(); i++) {
        for (int j = i + 1; j < grid.galaxies.size(); j++) {
            Cell* galaxyA = grid.galaxies[i];
            Cell* galaxyB = grid.galaxies[j];

            Vec2 expPosA = CalculateGalaxyExpandedPosition(galaxyA, grid);
            Vec2 expPosB = CalculateGalaxyExpandedPosition(galaxyB, grid);
            long yDiff = std::abs(expPosB.y - expPosA.y);
            long xDiff = std::abs(expPosB.x - expPosA.x);
            totalDistance +=  yDiff + xDiff;
        }
    }

    std::cout << "PART 1 ANSWER - Sum of distances between each pair of galaxies (post-expansion): " << totalDistance << "\n";

    // Part 2 - A much older galaxy...
    Grid grid2(parsedLines);
    ExpandSpace(grid2, 1000000);

    totalDistance = 0;
    for (int i = 0; i < grid2.galaxies.size(); i++) {
        for (int j = i + 1; j < grid2.galaxies.size(); j++) {
            Cell* galaxyA = grid2.galaxies[i];
            Cell* galaxyB = grid2.galaxies[j];

            Vec2 expPosA = CalculateGalaxyExpandedPosition(galaxyA, grid2);
            Vec2 expPosB = CalculateGalaxyExpandedPosition(galaxyB, grid2);
            long yDiff = std::abs(expPosB.y - expPosA.y);
            long xDiff = std::abs(expPosB.x - expPosA.x);
            totalDistance += yDiff + xDiff;
        }
    }

    std::cout << "PART 2 ANSWER - Sum of distances between each pair of galaxies (post-expansion x1000000): " << totalDistance << "\n";


}