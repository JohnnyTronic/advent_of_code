#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>

enum class GridObjectType {
    Rock,
    Wall,
};

class GridObject {
public:
    int id;
    GridObjectType type;
    Vec2 position;
    bool locked = false;

    GridObject(int id, GridObjectType type, Vec2 position) : id(id), type(type), position(position) {
        if (type == GridObjectType::Wall)
            this->locked = true;
    };    

    std::string ToString() {
        return "(" + std::to_string(position.x) + "," + std::to_string(position.y) + ")";
    }
};

char GetCharForGridObject(GridObject* gridObject) {
    if (gridObject == nullptr)
        return '.';
    switch (gridObject->type) {
    case GridObjectType::Rock: {return 'O'; break; }
    case GridObjectType::Wall: {return '#'; break; }
    }

    return 'X';
}

class Grid {
public:
    std::vector<std::vector<GridObject*>> content;
    int width = 0;
    int height = 0;

    Grid(int width, int height) {
        for (int y = 0; y < height; y++) {
            std::vector<GridObject*> row;
            for (int x = 0; x < width; x++)
                row.push_back(nullptr);
            content.push_back(row);
        }

        this->width = content[0].size();
        this->height = content.size();
    }

    GridObject* GetGridObjectAt(Vec2& position) {
        return content[position.y][position.x];
    }

    void SetGridObjectAt(Vec2& position, GridObject* gridObject) {
        content[position.y][position.x] = gridObject;
    }

    void PopulateGridObjects(std::vector<GridObject*>& rocks, std::vector<GridObject*>& walls) {
        for (auto rock : rocks) {
            SetGridObjectAt(rock->position, rock);
        }

        for (auto wall : walls) {
            SetGridObjectAt(wall->position, wall);
        }
    }

    void PrintGrid(std::string title) {
        std::cout << "---" << title << "---\n";

        for (int y = 0; y < content.size(); y++) {
            auto& row = content[y];
            for (int x = 0; x < row.size(); x++) {
                auto pos = Vec2(x, y);
                auto gridObject = GetGridObjectAt(pos);
                char goChar = GetCharForGridObject(gridObject);
                std::cout << goChar;
            }
            std::cout << "\n";
        }

        std::cout << "----------\n";
    }
};

void Tilt(std::vector<GridObject*>& rocks, Grid& grid, const Vec2& direction) {

    // Unlock rocks
    std::list<GridObject*> unlockedRocks(rocks.begin(), rocks.end());
    for (auto rock : rocks) {
        rock->locked = false;        
    }
        
    while (unlockedRocks.size() > 0) {        
        for (std::list<GridObject*>::iterator iterator = unlockedRocks.begin(); iterator != unlockedRocks.end();) {
            GridObject* rock = *iterator;
            Vec2 nextPosition = rock->position + direction;

            if (nextPosition.x < 0 || nextPosition.x >= grid.width || nextPosition.y < 0 || nextPosition.y >= grid.height) {
                // Next position would put us out of bounds. Don't move.
                rock->locked = true;
                iterator = unlockedRocks.erase(iterator);
                //rocks.erase(unlockedRocks.begin() + r);
                continue;
            }

            GridObject* nextGridObject = grid.GetGridObjectAt(nextPosition);            
            if (nextGridObject == nullptr) {
                // Move rock forward into empty space
                grid.SetGridObjectAt(rock->position, nullptr);
                rock->position = nextPosition;
                grid.SetGridObjectAt(rock->position, rock);   
                iterator++;
                continue;
            }
            else {
                if (nextGridObject->locked) {
                    // Rock hit a locked object, lock the rock
                    rock->locked = true;
                    iterator = unlockedRocks.erase(iterator);
                    continue;
                }
            }
            // Else, rock is blocked by a still moving object. Wait for next simulation step to see if obstacle has itself moved.
            iterator++;
        }
    }
}

void SpinCycle(std::vector<GridObject*>& rocks, Grid& grid) {
    Tilt(rocks, grid, Vec2(0, -1));
    Tilt(rocks, grid, Vec2(-1, 0));
    Tilt(rocks, grid, Vec2(0, 1));
    Tilt(rocks, grid, Vec2(1, 0));    
}

std::string GenerateGridSnapshot(Grid& grid) {
    std::string rockPositionRecord("");
    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            Vec2 pos(x, y);
            auto go = grid.GetGridObjectAt(pos);
            if (go == nullptr)
                continue;
            else {
                if (go->type == GridObjectType::Rock) {
                    rockPositionRecord += go->ToString() + "\n";
                }
            }
        }
    }   
    return rockPositionRecord;
}

long long CalculateTotalLoadNorth(std::vector<GridObject*> rocks, Grid& grid) {
    long long totalLoad = 0;
    for (auto& rock : rocks) {
        long long rockLoad = grid.height - rock->position.y;
        totalLoad += rockLoad;
    }
    return totalLoad;
}

Grid ParseInputs(std::string inputFile, std::vector<GridObject*>& rocks, std::vector<GridObject*>& walls) {
    rocks.clear();
    walls.clear();
    std::ifstream ifs(inputFile);
    std::string parsedLine;
    int y = 0;
    int objectId = 0;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        for (int x = 0; x < parsedLine.size(); x++) {
            if (parsedLine[x] == 'O') {
                GridObject* rock = new GridObject(objectId++, GridObjectType::Rock, Vec2(x, y));
                rocks.push_back(rock);
            }

            if (parsedLine[x] == '#') {
                GridObject* wall = new GridObject(objectId++, GridObjectType::Wall, Vec2(x, y));
                walls.push_back(wall);
            }
        }
        y++;
    }

    Grid grid(parsedLine.size(), y);
    grid.PopulateGridObjects(rocks, walls);
    return grid;
}

int main()
{
    std::cout << "Advent of Code - Day 14!\n";

    std::vector<GridObject*> rocks;
    std::vector<GridObject*> walls;
    Grid grid = ParseInputs("test_input.txt", rocks, walls);
        
    grid.PrintGrid(std::string("Starting Grid"));

    std::cout << "Grid tilted north\n";
    Tilt(rocks, grid, Vec2(0, -1));
    grid.PrintGrid(std::string("Starting Grid Tilted North"));

    long long totalLoadNorth = CalculateTotalLoadNorth(rocks, grid);

    std::cout << "PART 1 ANSWER - Total load on the north support beams: " << totalLoadNorth << "\n";


    // Part 2 - A million spin cycles
    Grid spinGrid = ParseInputs("input.txt", rocks, walls);
    spinGrid.PrintGrid(std::string("Starting Grid"));
    long long maxSpinCount = 1000000000;
    long long actualSpinCount = 0;
    //long long spinCount = 3;
    std::map<std::string, long long> positionRecordSet;
    std::string printTitle = "-- - Loop-- - ";
    for (long long i = 0; i < maxSpinCount; i++) {
        SpinCycle(rocks, spinGrid);
        actualSpinCount++;      

        std::string rockPositionRecord = GenerateGridSnapshot(spinGrid);
        if (positionRecordSet.contains(rockPositionRecord)) {
            // Cycle detected
            std::cout << "Cycle detected at loop: " << i << "\n";
            long long cycleStartLoopNumber = positionRecordSet[rockPositionRecord];
            long long cycleLength = i - cycleStartLoopNumber;                
            long long remaining = maxSpinCount - i;
            long long modRemaining = remaining % cycleLength;
            for (int i = 0; i < modRemaining - 1; i++) {
                SpinCycle(rocks, spinGrid);                    
            }
            break;
        }
        else {
            positionRecordSet[rockPositionRecord] = i;  
        }
    }

    totalLoadNorth = CalculateTotalLoadNorth(rocks, spinGrid);

    std::cout << "PART 2 ANSWER - Total load on north support beams after " << actualSpinCount << " spin cycles: " << totalLoadNorth << "\n";
   
    return 0;
}