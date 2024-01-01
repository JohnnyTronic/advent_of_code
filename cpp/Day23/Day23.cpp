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

std::vector<Vec2> NeighbourOffsets{ Vec2(0,-1), Vec2(1,0), Vec2(0,1), Vec2(-1,0) };

std::vector<Vec2> GetValidNeighbours(const Vec2& position, const Grid& grid, std::unordered_map<std::size_t, long long> visited) {

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

        auto neighbour = grid.GetAt(neighbourPosition);

        // Can't walk into forest
        if (neighbour->terrain == FOREST)
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

struct Candidate {
public:
    Vec2 position;
    int distance;
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
    std::unordered_map<std::size_t, long long> visited;

    Candidate longestPath;
    visited[startPosition.Hash()] =  0;
    frontier.push(Candidate(startPosition,0));
    while (frontier.size() > 0) {
        Candidate focusCandidate = frontier.top();
        frontier.pop();
        long long focusDistance = visited.at(focusCandidate.position.Hash());

        if (focusCandidate.position == endPosition) {
            longestPath = focusCandidate;
            break;
        }

        auto validNeighbours = GetValidNeighbours(focusCandidate.position, grid, visited);
        for (const auto& neighbour : validNeighbours) {
            frontier.push(Candidate(neighbour, focusCandidate.distance + 1));
            visited[neighbour.Hash()] = focusDistance + 1;
        }
    }

    std::cout << "PART 1 ANSWER - Longest walking path: " << longestPath.distance << "\n";
}

int main()
{
    std::cout << "Advent of Code - Day 23!\n";

    std::string inputFile = "test_input.txt";
    DoPart1(inputFile);
}