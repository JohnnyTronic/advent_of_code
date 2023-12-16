#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

class Rock {
public:
    Vec2 position;

    Rock(Vec2 position) : position(position) {

    }
};

class Wall {
public:
    Vec2 position;

    Wall(Vec2 position) : position(position) {};
};

class Grid {
public:
    std::vector<std::vector<char>> content;
    Grid(int width, int height) {
        for (int y = 0; y < height; y++) {
            std::vector<char> row;
            for (int x = 0; x < width; x++) {
                row.push_back('.');
            }
            content.push_back(row);
        }
    }

    char GetCharAt(Vec2& position) {
        return content[position.y][position.x];
    }

    void SetCharAt(Vec2& position, char character) {
        content[position.y][position.x] = character;
    }

    int GetWidth() {
        return content[0].size();
    }

    int GetHeight() {
        return content.size();
    }

    void PopulateSymbols(std::vector<Rock>& rocks, std::vector<Wall>& walls) {
        for (auto& rock : rocks) {
            SetCharAt(rock.position, 'O');
        }

        for (auto& wall : walls) {
            SetCharAt(wall.position, '#');
        }
    }

    void PrintGrid() {
        std::cout << "---Grid---\n";

        for (int y = 0; y < content.size(); y++) {
            auto& row = content[y];
            for (int x = 0; x < row.size(); x++) {
                auto pos = Vec2(x, y);
                std::cout << GetCharAt(pos);
            }
            std::cout << "\n";
        }

        std::cout << "----------\n";
    }
};

void Tilt(std::vector<Rock>& rocks, std::vector<Wall>& walls, Grid& grid, Vec2 direction) {

    bool rocksStillMoving = true;
    while (rocksStillMoving) {
        rocksStillMoving = false;

        for (auto& rock : rocks) {
            Vec2 nextPosition = rock.position + direction;

            if (nextPosition.x < 0 || nextPosition.x >= grid.GetWidth() || nextPosition.y < 0 || nextPosition.y >= grid.GetHeight()) {
                // Next position would put us out of bounds. Don't move.
                continue;
            }

            if (grid.GetCharAt(nextPosition) == '.') {
                grid.SetCharAt(rock.position, '.');
                grid.SetCharAt(nextPosition, 'O');
                rock.position = nextPosition;
                rocksStillMoving = true;
            }
        }
    }
}

long long CalculateTotalLoadNorth(std::vector<Rock>& rocks, Grid& grid) {
    long long totalLoad = 0;
    for (auto& rock : rocks) {
        long long rockLoad = grid.GetHeight() - rock.position.y;
        totalLoad += rockLoad;
    }
    return totalLoad;
}

int main()
{
    std::cout << "Advent of Code - Day 14!\n";

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");
        
    std::vector<Rock> rocks;
    std::vector<Wall> walls;
    std::string parsedLine;
    int y = 0;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        for (int x = 0; x < parsedLine.size(); x++) {
            if (parsedLine[x] == 'O') {
                Rock rock(Vec2(x, y));
                rocks.push_back(rock);
            }

            if (parsedLine[x] == '#') {
                Wall wall(Vec2(x, y));
                walls.push_back(wall);
            }
        }
        y++;
    }

    Grid grid(parsedLine.size(), y);
    grid.PopulateSymbols(rocks, walls);
    std::cout << "Starting grid\n";
    grid.PrintGrid();

    std::cout << "Grid tilted north";
    Tilt(rocks, walls, grid, Vec2(0, -1));
    grid.PrintGrid();

    long long totalLoadNorth = CalculateTotalLoadNorth(rocks, grid);

    std::cout << "PART 1 ANSWER - Total load on the north support beams: " << totalLoadNorth << "\n";
   
    return 0;
}