#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>

class Coord2D {
public:
    int x = 0;
    int y = 0;
    int gridWidth;
    int gridHeight;

    Coord2D(int x, int y, int gridWidth, int gridHeight) : x(x), y(y), gridWidth(gridWidth), gridHeight(gridHeight) {};

    bool isOnGrid() {
        return x >= 0 && x < gridWidth && y >= 0 && y < gridHeight;
    }

    int ToCoord1D() {
        return y * gridWidth + x;
    }
};

int Coord2Dto1D(Coord2D coord, int gridWidth, int gridHeight) {
    return coord.y * gridWidth + coord.x;
}

Coord2D Coord1Dto2D(int index, int gridWidth, int gridHeight) {
    Coord2D coord(index % gridWidth, index / gridWidth, gridWidth, gridHeight);   
    return coord;
}

std::vector<int> GenerateNeighbourIndicies(int index, int gridWidth, int gridHeight) {
    
    Coord2D targetCell = Coord1Dto2D(index, gridWidth, gridHeight);

    std::vector<int> neighbourIndicies;

    Coord2D topLeft = Coord2D(targetCell.x - 1, targetCell.y - 1, gridWidth, gridHeight);
    if (topLeft.isOnGrid())
        neighbourIndicies.push_back(topLeft.ToCoord1D());

    Coord2D top = Coord2D(targetCell.x, targetCell.y - 1, gridWidth, gridHeight);
    if (top.isOnGrid())
        neighbourIndicies.push_back(top.ToCoord1D());

    Coord2D topRight = Coord2D(targetCell.x + 1, targetCell.y - 1, gridWidth, gridHeight);
    if (topRight.isOnGrid())
        neighbourIndicies.push_back(topRight.ToCoord1D());

    Coord2D left = Coord2D(targetCell.x - 1, targetCell.y, gridWidth, gridHeight);
    if (left.isOnGrid())
        neighbourIndicies.push_back(left.ToCoord1D());

    Coord2D right = Coord2D(targetCell.x + 1, targetCell.y, gridWidth, gridHeight);
    if (right.isOnGrid())
        neighbourIndicies.push_back(right.ToCoord1D());

    Coord2D bottomLeft = Coord2D(targetCell.x - 1, targetCell.y + 1, gridWidth, gridHeight);
    if (bottomLeft.isOnGrid())
        neighbourIndicies.push_back(bottomLeft.ToCoord1D());

    Coord2D bottom = Coord2D(targetCell.x, targetCell.y + 1, gridWidth, gridHeight);
    if (bottom.isOnGrid())
        neighbourIndicies.push_back(bottom.ToCoord1D());

    Coord2D bottomRight = Coord2D(targetCell.x + 1, targetCell.y + 1, gridWidth, gridHeight);
    if (bottomRight.isOnGrid())
        neighbourIndicies.push_back(bottomRight.ToCoord1D());
    
    return neighbourIndicies;
}

int main()
{
    std::cout << "Advent of Code 2023 - Day 3\n";

    int sumOfPartNumbers = 0;

    std::ifstream ifs("test_input.txt");
    //std::ifstream ifs("input.txt");
    std::string fileContent;
    std::string line;    

    int gridWidth = 0;
    int gridHeight = 0;
    while (std::getline(ifs, line)) {
        fileContent += line;
        gridWidth = line.length();
        gridHeight++;
    }

    std::set<int> visitedCells;
        
    int nextSymbolIndex = fileContent.find_first_not_of("'0123456789.");
    while (nextSymbolIndex != std::string::npos) {
        std::cout << "Next symbol index: " << nextSymbolIndex << std::endl;
        nextSymbolIndex = fileContent.find_first_not_of("'0123456789.", nextSymbolIndex + 1);
    }

    
    
    
    std::cout << "Answer for Part 1 - Sum of part numbers: " << sumOfPartNumbers << std::endl;
}