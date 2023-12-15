#include "Grid.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

void ExpandSpace(Grid& grid) {
    // Expand empty columns
    for (int x = 0; x < grid.GetWidth(); x++) {
        const auto& column = grid.GetColumn(x);
        bool emptySpace = true;
        for (auto cell : column) {
            if (cell->content != '.') {
                emptySpace = false;
                break;
            }
        }
        if (emptySpace) {
            std::cout << "Found empty column: " << x << "\n";
            grid.InsertColumnBefore(x);
            x++;
        }
    }

    // Expand empty rows
    for (int y = 0; y < grid.GetHeight(); y++) {
        const auto& row = grid.GetRow(y);
        bool emptySpace = true;
        for (auto cell : row) {
            if (cell->content != '.') {
                emptySpace = false;
                break;
            }
        }
        if (emptySpace) {
            std::cout << "Found empty row: " << y << "\n";
            grid.InsertRowBefore(y);
            y++;
        }
    }
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

    std::cout << "Grid BEFORE expansion\n";
    grid.PrintGrid();
        
    ExpandSpace(grid);
    grid.RefreshCellPositionData();
    
    std::cout << "Grid AFTER expansion\n";   
    grid.PrintGrid();

    std::vector<Cell*> galaxies;
    for (int y = 0; y < grid.GetHeight(); y++) {
        for (int x = 0; x < grid.GetWidth(); x++) {
            auto cell = grid.GetCellAt(Vec2(x, y));
            if (cell->content == '#')
                galaxies.push_back(cell);
        }
    }

    long totalDistance = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            Cell* cellA = galaxies[i];
            Cell* cellB = galaxies[j];

            long yDiff = std::abs(cellB->position.y - cellA->position.y);
            long xDiff = std::abs(cellB->position.x - cellA->position.x);
            totalDistance +=  yDiff + xDiff;
        }
    }

    std::cout << "PART 1 ANSWER - Sum of distances between each pair of galaxies (post-expansion): " << totalDistance << "\n";

}