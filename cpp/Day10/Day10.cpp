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

Vec2 FindStartCoordinate(Grid& grid) {
    Vec2 startCoord = Vec2(0, 0);
    for (int x = 0; x < grid.GetWidth(); x++) {
        for (int y = 0; y < grid.GetHeight(); y++) {
            auto c = grid.GetCharAt(Vec2(x, y));
            if (c == 'S') {
                startCoord = Vec2(x, y);
            }
        }
    }

    return startCoord;
}

bool DoesCellConnectToCell(Vec2& cellA, Vec2& cellB, Grid& grid) {
    char charA = grid.GetCharAt(cellA);
    auto offsets = GetPipeNeighbourOffsets(charA);
    for (auto& offset : offsets) {
        Vec2 resultantCoord = offset + cellA;
        if (resultantCoord == cellB) {
            return true;
        }
    }

    return false;
}

std::vector<Vec2> GetCoordsConnectedTo(Vec2 center, Grid& grid) {
    char centerChar = grid.GetCharAt(center);

    std::vector<Vec2> pipeNeighbours{};
    auto neighbourOffsets = GetPipeNeighbourOffsets(centerChar);
    for (auto offset : neighbourOffsets) {
        auto resultantCoord = center + offset;
        pipeNeighbours.push_back(resultantCoord);
    }

    return pipeNeighbours;
}

Vec2 GetCoordConnectedToCenterExcluding(Vec2 center, Grid& grid, Vec2 exclude) {
    auto connectedNeighbours = GetCoordsConnectedTo(center, grid);
    std::vector<Vec2> connectedNeighbourPostExclusion{};
    for (auto neighbour : connectedNeighbours)
    {
        if (neighbour != exclude)
            connectedNeighbourPostExclusion.push_back(neighbour);
    }
    
    if (connectedNeighbours.size() == 0) {
        throw "Should always have at least one pipe neighbour ahead of us...";
    }
    return connectedNeighbourPostExclusion[0];
}

class PipeWalker {
public:
    Vec2 position;
    Vec2 previousPosition;
    Grid& grid;
    long long steps = 1;

    PipeWalker(Vec2 position, Vec2 previousPosition, Grid& grid) : position(position), previousPosition(previousPosition), grid(grid) {}

    long long StepForward() {
        auto nextPosition = PreviewNextPosition();
        previousPosition = position;
        position = nextPosition;
        steps++;       

        return steps;
    }

    Vec2 PreviewNextPosition() {
        auto nextPosition = GetCoordConnectedToCenterExcluding(position, grid, previousPosition);
        return nextPosition;
    }
};

int main()
{
    std::cout << "Advent of Code - Day 10!\n";
    
    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    std::vector<std::string> parsedLines;
    std::string parsedLine;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    Grid grid(parsedLines);
    
    Vec2 startCoord = FindStartCoordinate(grid);
    std::cout << "Found starting coord: " << startCoord << "\n";
    std::vector<Vec2> startingNeighbours = grid.GetNeighbourCoords(startCoord);
    std::vector<Vec2> cellsConnectedToStart;
    std::copy_if(startingNeighbours.begin(), startingNeighbours.end(), std::back_inserter(cellsConnectedToStart), [&startCoord, &grid](Vec2 v) {return DoesCellConnectToCell(v, startCoord, grid); });
    std::vector<PipeWalker> pipeWalkers{};
    for (auto cell : cellsConnectedToStart) {
        PipeWalker walker(cell, startCoord, grid);
        pipeWalkers.push_back(walker);
    }
            
    while (!(pipeWalkers[0].position == pipeWalkers[1].position)) {
        pipeWalkers[0].StepForward();
        pipeWalkers[1].StepForward();       
    }

    std::cout << "PART 1 ANSWER - How far does each pipe walker travel? " << pipeWalkers[0].steps << "\n";
    return 0;
}