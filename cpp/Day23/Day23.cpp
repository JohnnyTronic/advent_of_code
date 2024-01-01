#include "Vec2.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

enum Terrain {
    PATH,
    FOREST,
    NORTHSLOPE,
    EASTSLOPE,
    SOUTHSLOPE,
    WESTSLOPE
};

char TerrainToChar(Terrain terrain) {
    switch (terrain) {
    case PATH: return '.';
    case FOREST: return '#';
    case NORTHSLOPE: return '^';
    case EASTSLOPE: return '>';
    case SOUTHSLOPE: return 'v';
    case WESTSLOPE: return '<';
    default: throw "Unrecognized terrain.";
    }
}

struct Cell {
public:
    Terrain terrain;

    Cell(Terrain terrain) : terrain(terrain) {};
};

struct Grid {
public:  
    int width;
    int height;

    Grid(const std::vector<std::string>& input) {
        for (const auto& line : input) {
            std::vector<Cell*> row;
            for (const auto& c : line) {
                switch (c) {
                case '#': row.push_back(new Cell(FOREST)); break;
                case '.': row.push_back(new Cell(PATH)); break;
                case '^': row.push_back(new Cell(NORTHSLOPE)); break;
                case '>': row.push_back(new Cell(EASTSLOPE)); break;
                case 'v': row.push_back(new Cell(SOUTHSLOPE)); break;
                case '<': row.push_back(new Cell(WESTSLOPE)); break;
                default: throw "Unrecognized input.";
                }
            }
            content.push_back(row);
        }

        width = content[0].size();
        height = content.size();
    }

    Cell* GetAt(int x, int y) const {
        return content[y][x];
    }
    Cell* GetAt(Vec2 position) const {
        return GetAt(position.x, position.y);
    }

    bool IsWithinBounds(Vec2 position) const {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

    void PrintGrid() {
        std::cout << "---Grid---\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::cout << TerrainToChar(GetAt(x, y)->terrain);
            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    }

private:
    std::vector<std::vector<Cell*>> content;
};

Grid ParseInput(const std::string& fileName) {
  
    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedLines;
   
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
      
    }

    Grid grid(parsedLines);
    return grid;
}

struct Candidate {
public:
    Vec2 position;
    int distance;

    long long Hash() const {
        return position.x + (position.y << 8) + (distance << 16);
    }
};

std::vector<Vec2> NeighbourOffsets{ Vec2(0,-1), Vec2(1,0), Vec2(0,1), Vec2(-1,0) };

std::vector<Vec2> GetValidNeighbours(const Vec2& position, const Grid& grid, std::unordered_set<long long> visited) {

    auto currentCell = grid.GetAt(position);
    std::vector<Vec2> neighbourPositions;

    for (const auto& offset : NeighbourOffsets) {        
        Vec2 neighbourPosition = position + offset;
        
        // Can't walk outside of map
        if(!grid.IsWithinBounds(neighbourPosition))
            continue;

        // Can't backtrack
        if (visited.contains(neighbourPosition.Hash()))
            continue;

        auto neighbourCell = grid.GetAt(neighbourPosition);

        // Can't walk into forest
        if (neighbourCell->terrain == FOREST)
            continue;

        if (currentCell->terrain == NORTHSLOPE && offset != NORTH)
            continue;
        if (currentCell->terrain == EASTSLOPE && offset != EAST)
            continue;
        if (currentCell->terrain == SOUTHSLOPE && offset != SOUTH)
            continue;
        if (currentCell->terrain == WESTSLOPE && offset != WEST)
            continue;

        neighbourPositions.push_back(neighbourPosition);
    }

    return neighbourPositions;
}

class Walker {
public:
    Vec2 position;
    int steps;
    std::unordered_set<long long> visited;

    Walker(Vec2 position, int steps, std::unordered_set<long long> visited) : position(position), steps(steps), visited(visited) {
        this->visited.insert(position.Hash());
    }

    void MoveTo(Vec2 nextPosition) {
        position = nextPosition;
        steps++;
        visited.insert(nextPosition.Hash());
    }

    Walker* Clone() {
        Walker* newWalker = new Walker(position, steps, visited);
        return newWalker;
    }
};

void DoPart1(const std::string& fileName) {
    Grid grid = ParseInput(fileName);

    Vec2 startPosition(1, 0);
    Vec2 endPosition(grid.width - 2, grid.height - 1);

    //grid.PrintGrid();

    // Find longest path from start to end
    struct CustomComparison {
        bool operator()(const Candidate lhs, const Candidate rhs) const { return lhs.distance < rhs.distance; }
    };
    std::priority_queue<Candidate, std::vector<Candidate>, CustomComparison> frontier;
    std::unordered_set<long long> visited;

    std::vector<Walker*> allWalkers{ new Walker(startPosition, 0, visited) };
    std::queue<Walker*> activeWalkers;
    for (auto walker : allWalkers)
        activeWalkers.push(walker);
     
    while (activeWalkers.size() > 0) {
        Walker* focusWalker = activeWalkers.front();

        if (focusWalker->position == endPosition) {
            activeWalkers.pop();
            continue;
        }

        auto validNeighbours = GetValidNeighbours(focusWalker->position, grid, focusWalker->visited);

        if (validNeighbours.size() == 0) {
            activeWalkers.pop();
            
            std::remove_if(allWalkers.begin(), allWalkers.end(), [&focusWalker](Walker* walker) {return walker == focusWalker; });
            continue;
        }
        else if (validNeighbours.size() == 1) {
            focusWalker->MoveTo(validNeighbours[0]);
            continue;
        }
        else {
            for (int i = 1; i < validNeighbours.size(); i++) {
                Walker* newWalker = focusWalker->Clone();
                newWalker->MoveTo(validNeighbours[i]);
                activeWalkers.push(newWalker);
                allWalkers.push_back(newWalker);
            }
            focusWalker->MoveTo(validNeighbours[0]);
        }
    }

    int longestPath = allWalkers[0]->steps;
    Walker* longestWalker = allWalkers[0];
    for (int i = 1; i < allWalkers.size(); i++) {
        Walker* testWalker = allWalkers[i];
        if (testWalker->steps > longestPath) {
            longestPath = testWalker->steps;
            longestWalker = testWalker;
        }
    }

    std::cout << "PART 1 ANSWER - Longest walking path: " << longestWalker->steps << "\n";
}

int main()
{
    std::cout << "Advent of Code - Day 23!\n";

    std::string inputFile = "input.txt";
    DoPart1(inputFile);
}