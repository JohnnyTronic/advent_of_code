#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
#include <vector>
#include <map>
#include <memory>

enum CellContent {
    START,
    GARDEN,
    ROCK
};

CellContent GetCellContentForChar(const char& c) {
    switch (c) {
    case 'S': return START;
    case '.': return GARDEN;
    case '#': return ROCK;
    default: throw "Unrecognized cell char: " + c;
    }
}

class Vec2 {
public:
    int x = 0;
    int y = 0;

    Vec2(int x, int y) : x(x), y(y) {}
};

class Cell {
public:
    CellContent content;
    bool isReachable = false;

    Cell(CellContent content) : content(content) {};
};

class Grid {
public:
    int width;
    int height;
    Grid(const std::vector<std::string>& inputStrings) {
        for (const auto& line : inputStrings) {
            std::vector<Cell> row;
            for (const auto& c : line) {
                Cell cell(GetCellContentForChar(c));
                row.push_back(cell);
            }
            content.push_back(row);
        }

        width = content[0].size();
        height = content.size();
    }

    Cell& GetCell(Vec2& position) {
        return content[position.y][position.x];
    }

    bool IsWithinBounds(Vec2& position) {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

private:
    std::vector<std::vector<Cell>> content;
};

Grid ParseInput(const std::string& fileName) {

    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    std::regex wholeModulePattern(R"((.+)\s->\s(.+))");
    std::regex moduleNamePattern(R"(([%&])?(\w+))");
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    Grid grid(parsedLines);

    return grid;
}

void DoPart1(Grid& grid) {

    long long totalReachableSpaces = 0;


    std::cout << "PART 1 ANSWER - How many spaces are reachable in 64 steps?: " << totalReachableSpaces << "\n";
}

void DoPart2(Grid& grid) {
    // Not yet implemented
}

int main()
{
    std::cout << "Advent of Code - Day 21!\n";

    Grid grid = ParseInput("input.txt");

    DoPart1(grid);
    DoPart2(grid);
}