#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>

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
    std::string northColor{ "" };
    std::string eastColor{ "" };
    std::string southColor{ "" };
    std::string westColor{ "" };
};

template <typename T>
class Grid {
public:
    std::map<int, std::map<int, T>> content;

    T Get(Vec2 position) {   
        return content[position.y][position.x];
    }

    void Set(Vec2 position, T cell) {
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
                std::cout << cell->isHollow ? "#" : ".";
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

void PaintCell(Cell* cell, Instruction& instruction) {
    if (instruction.direction == Direction::NORTH || instruction.direction == Direction::SOUTH) {
        cell->eastColor = instruction.hexColor;
        cell->westColor = instruction.hexColor;
    }
    else if (instruction.direction == Direction::WEST || instruction.direction == Direction::EAST) {
        cell->northColor = instruction.hexColor;
        cell->southColor = instruction.hexColor;
    }
}

int main()
{
    std::cout << "Advent of Code - Day 18!\n";

    std::ifstream ifs("test_input.txt");
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

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
    
    long long lagoonVolume = 0;
    std::vector<Vec2> vertices;
    {
        Vec2 currentPosition(0, 0);
        vertices.push_back(currentPosition);
        for (auto& instruction : instructions) {
            auto directionVector = DirectionToVec2(instruction.direction);
            currentPosition = currentPosition + (directionVector * instruction.distance);
            vertices.push_back(currentPosition);
        }
    }
    
    auto extents = GetExtents(vertices);

    std::vector<Edge> edges;
    for (int i = 0; i < vertices.size() - 1; i++) {
        Edge edge(vertices[i], vertices[i + 1]);
        edges.push_back(edge);
    }    
    Edge finalEdge(vertices.back(), vertices.front());
    edges.push_back(finalEdge);

    Grid<Cell*> grid;
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
            PaintCell(cell, instruction);
        }
    }
    
    grid.Print(extents);
    

    std::cout << "ANSWER PART 1 - What is the lagoon's volume?: " << std::to_string(lagoonVolume) << "\n";
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
