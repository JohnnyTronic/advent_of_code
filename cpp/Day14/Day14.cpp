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
    int width = 0;
    int height = 0;

    Grid(int width, int height) {
        for (int y = 0; y < height; y++) {
            std::vector<char> row;
            for (int x = 0; x < width; x++) {
                row.push_back('.');
            }
            content.push_back(row);
        }

        this->width = content[0].size();
        this->height = content.size();
    }

    char GetCharAt(Vec2& position) {
        return content[position.y][position.x];
    }

    void SetCharAt(Vec2& position, char character) {
        content[position.y][position.x] = character;
    }

    void PopulateSymbols(std::vector<Rock>& rocks, std::vector<Wall>& walls) {
        for (auto& rock : rocks) {
            SetCharAt(rock.position, 'O');
        }

        for (auto& wall : walls) {
            SetCharAt(wall.position, '#');
        }
    }

    void PrintGrid(std::string title) {
        std::cout << "---" << title << "---\n";

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

            if (nextPosition.x < 0 || nextPosition.x >= grid.width || nextPosition.y < 0 || nextPosition.y >= grid.height) {
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

void SpinCycle(std::vector<Rock>& rocks, std::vector<Wall>& walls, Grid& grid) {
    Tilt(rocks, walls, grid, Vec2(0, -1));
    Tilt(rocks, walls, grid, Vec2(-1, 0));
    Tilt(rocks, walls, grid, Vec2(0, 1));
    Tilt(rocks, walls, grid, Vec2(1, 0));
}

long long CalculateTotalLoadNorth(std::vector<Rock>& rocks, Grid& grid) {
    long long totalLoad = 0;
    for (auto& rock : rocks) {
        long long rockLoad = grid.height - rock.position.y;
        totalLoad += rockLoad;
    }
    return totalLoad;
}

Grid ParseInputs(std::string inputFile, std::vector<Rock>& rocks, std::vector<Wall>& walls) {
    rocks.clear();
    walls.clear();
    std::ifstream ifs(inputFile);
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
    return grid;
}

int main()
{
    std::cout << "Advent of Code - Day 14!\n";

    std::vector<Rock> rocks;
    std::vector<Wall> walls;
    //Grid grid = ParseInputs("test_input.txt", rocks, walls);
    Grid grid = ParseInputs("input.txt", rocks, walls);
        
    grid.PrintGrid(std::string("Starting Grid"));

    std::cout << "Grid tilted north\n";
    Tilt(rocks, walls, grid, Vec2(0, -1));
    grid.PrintGrid(std::string("Starting Grid Tilted North"));

    long long totalLoadNorth = CalculateTotalLoadNorth(rocks, grid);

    std::cout << "PART 1 ANSWER - Total load on the north support beams: " << totalLoadNorth << "\n";

    // Part 2 - A million spin cycles
    Grid spinGrid = ParseInputs("test_input.txt", rocks, walls);
    spinGrid.PrintGrid(std::string("Starting Grid"));
    long long spinCount = 1000000000;
    //long long spinCount = 3;
    for (long long i = 0; i < spinCount; i++) {
        SpinCycle(rocks, walls, spinGrid);
        //std::string printTitle = "Spin " + std::to_string(i) + "\n";
        //spinGrid.PrintGrid(printTitle);

        if (i % 1000000 == 0)
            std::cout << "Spins: " << i << "\n";
    }

    std::string postSpinTitle = "Grid After Spin Count: ";
    postSpinTitle += std::to_string(spinCount);

    spinGrid.PrintGrid(std::string(postSpinTitle));
    totalLoadNorth = CalculateTotalLoadNorth(rocks, spinGrid);

    std::cout << "PART 2 ANSWER - Total load on north support beams after " << spinCount << " spin cycles: " << totalLoadNorth << "\n";
   
    return 0;
}