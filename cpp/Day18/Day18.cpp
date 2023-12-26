#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <cassert>

enum class Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

Direction CharToDirection(const char c) {
    switch (c) {
    case 'U': return Direction::NORTH;
    case 'R': return Direction::EAST;
    case 'D': return Direction::SOUTH;
    case 'L': return Direction::WEST;
    default: throw;
    }
}

struct Instruction {
public:
    Direction direction;
    std::size_t distance;
    std::string hexColor;

    Instruction(Direction direction, std::size_t distance, std::string hexColor) : direction(direction), distance(distance), hexColor(hexColor) {}
};

Vec2 DirectionToVec2(Direction direction) {
    switch (direction) {
    case Direction::NORTH: return NORTH;
    case Direction::EAST: return EAST;
    case Direction::SOUTH: return SOUTH;
    case Direction::WEST: return WEST;
    default: throw;
    }
}

struct Extents {
public:
    int xMin{};
    int xMax{};
    int yMin{};
    int yMax{};
};

Extents GetExtents(const std::vector<Vec2>& vertices) {
    Extents extents;
    for (auto vertex : vertices) {
        extents.xMin = std::min(extents.xMin, vertex.x);
        extents.xMax = std::max(extents.xMax, vertex.x);
        extents.yMin = std::min(extents.yMin, vertex.y);
        extents.yMax = std::max(extents.yMax, vertex.y);
    }
    return extents;
}

class Cell {
public:
    bool isHollow{ false };
    bool isTrench{ false };
    std::string northColor{ "" };
    std::string eastColor{ "" };
    std::string southColor{ "" };
    std::string westColor{ "" };
};

class Grid {
public:
    std::map<int, std::map<int, Cell*>> content;

    Cell* Get(Vec2 position) {   
        return content[position.y][position.x];
    }

    void Set(Vec2 position, Cell* cell) {
        content[position.y][position.x] = cell;
    }

    void Print(Extents extents) {
        std::cout << "---Grid---\n";
        for (int y = extents.yMin; y <= extents.yMax; y++) {
            for (int x = extents.xMin; x <= extents.xMax; x++) {
                Cell* cell = Get(Vec2(x, y));
                if (cell == nullptr) {
                    std::cout << ".";
                    continue;
                }
                if (cell->isTrench)
                    std::cout << "#";
                else if (cell->isHollow)
                    std::cout << "X";
                else
                    std::cout << ".";                
            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    }
};

class Edge {
public:
    Vec2 vertexA;
    Vec2 vertexB;

    Edge(Vec2 vertexA, Vec2 vertexB) : vertexA(vertexA), vertexB(vertexB) {};
};

void PaintCell(Cell* cell, const Instruction& instruction) {
    if (instruction.direction == Direction::NORTH || instruction.direction == Direction::SOUTH) {
        cell->eastColor = instruction.hexColor;
        cell->westColor = instruction.hexColor;
    }
    else if (instruction.direction == Direction::WEST || instruction.direction == Direction::EAST) {
        cell->northColor = instruction.hexColor;
        cell->southColor = instruction.hexColor;
    }
}

Grid CutTrench(const std::vector<Instruction>& instructions) {
    Grid grid;
    Vec2 currentPosition{};
    for (auto& instruction : instructions) {
        Vec2 direction = DirectionToVec2(instruction.direction);
        Vec2 startPosition = currentPosition;
        for (int i = 0; i <= instruction.distance; i++) {
            currentPosition = startPosition + direction * i;
            Cell* cell = grid.Get(currentPosition);
            if (cell == nullptr) {
                cell = new Cell();
                grid.Set(currentPosition, cell);
            }           
            cell->isHollow = true;
            cell->isTrench = true;
            PaintCell(cell, instruction);
        }
    }

    return grid;
}

bool IsDegenerateCorner(Vec2 scanPos, Grid& grid) {
    Cell* scanCell = grid.Get(scanPos);
    if (scanCell == nullptr || !scanCell->isTrench)
        return false;

    Cell* northNeighbour = grid.Get(scanPos + Vec2(0, -1));
    Cell* eastNeighbour = grid.Get(scanPos + Vec2(1, 0));
    if (northNeighbour != nullptr && northNeighbour->isTrench && eastNeighbour != nullptr && eastNeighbour->isTrench)
        return true;

    Cell* westNeighbour = grid.Get(scanPos + Vec2(-1, 0));
    Cell* southNeighbour = grid.Get(scanPos + Vec2(0, 1));
    if (westNeighbour != nullptr && westNeighbour->isTrench && southNeighbour != nullptr && southNeighbour->isTrench)
        return true;

    return false;
}

long long ScanDiagonalDownRight(Vec2 scanStartPos, Grid& grid, Extents& extents) {
    Vec2 scanDirection(1, 1);
    bool isInterior = false;   
    Vec2 scanPos = scanStartPos;
    long long totalInteriorCells = 0;
    while (scanPos.x <= extents.xMax && scanPos.y <= extents.yMax + 1) {
        Cell* scanCell = grid.Get(scanPos);

        if (scanCell != nullptr && scanCell->isTrench){
            totalInteriorCells++;
            if (!IsDegenerateCorner(scanPos, grid)) {
                isInterior = !isInterior;
            }            
        }
        else if (scanCell == nullptr && isInterior) {
            Cell* interiorCell = new Cell();
            interiorCell->isHollow = true;
            grid.Set(scanPos, interiorCell);
            totalInteriorCells++;
        }
        else if (scanCell != nullptr && isInterior) {
            scanCell->isHollow = true;
            totalInteriorCells++;
        }
        scanPos = scanPos + scanDirection;
    }
    return totalInteriorCells;
}

long long FillTrenchInterior(Grid& grid, Extents& extents) {

    // With a slight overscan, travel up the left edge then across the top edge of the extents
    // At each step, scan diagonally down-right, looking for trench crossings
    // and noting "degenerate" corners 7 and L
    long long totalHollowCells = 0;
    for (int y = extents.yMax + 1; y >= extents.yMin - 1; y--) {
        Vec2 scanStartPos(extents.xMin - 1, y);
        totalHollowCells += ScanDiagonalDownRight(scanStartPos, grid, extents);
    }

    for (int x = extents.xMin; x <= extents.xMax; x++) {
        Vec2 scanStartPos(x, extents.yMin - 1);
        totalHollowCells += ScanDiagonalDownRight(scanStartPos, grid, extents);
    }

    return totalHollowCells;
}

std::vector<std::string> ParseInput(const std::string& fileName) {
    std::ifstream ifs("input.txt");
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    return parsedLines;
}

std::vector<Instruction> ExtractInstructionsPart1(const std::vector<std::string>& parsedLines) {
    std::vector<Instruction> instructions;
    std::regex instructionPattern(R"((\w)\s(\d+)\s(.+))");
    for (const auto& line : parsedLines) {
        std::smatch match;
        std::regex_search(line.begin(), line.end(), match, instructionPattern);
        Direction direction = CharToDirection(match[1].str()[0]);
        std::size_t distance = std::stoi(match[2].str());
        std::string hexColor = match[3].str();
        Instruction instruction(direction, distance, hexColor);
        instructions.push_back(instruction);
    }

    return instructions;
}

std::vector<Vec2> ExtractVertices(const std::vector<Instruction>& instructions) {
    std::vector<Vec2> vertices;

    Vec2 currentPosition(0, 0);
    vertices.push_back(currentPosition);
    for (auto& instruction : instructions) {
        auto directionVector = DirectionToVec2(instruction.direction);
        currentPosition = currentPosition + (directionVector * instruction.distance);
        vertices.push_back(currentPosition);
    }

    return vertices;
}

std::vector<Edge> ComputeEdges(const std::vector<Vec2>& vertices) {
    std::vector<Edge> edges;

    for (int i = 0; i < vertices.size() - 1; i++) {
        Edge edge(vertices[i], vertices[i + 1]);
        edges.push_back(edge);
    }
    Edge finalEdge(vertices.back(), vertices.front());
    edges.push_back(finalEdge);

    return edges;
}

int main()
{
    std::cout << "Advent of Code - Day 18!\n";

    auto parsedLines = ParseInput("input.txt");
    
    std::vector<Instruction> instructions = ExtractInstructionsPart1(parsedLines);        
    std::vector<Vec2> vertices = ExtractVertices(instructions);       
    auto extents = GetExtents(vertices);
    std::vector<Edge> edges = ComputeEdges(vertices);    

    Grid grid = CutTrench(instructions);    
    //grid.Print(extents);

    long long lagoonVolume = FillTrenchInterior(grid, extents);    
    //grid.Print(extents);

    std::cout << "ANSWER PART 1 - What is the lagoon's volume?: " << std::to_string(lagoonVolume) << "\n";
    
    return 0;
}