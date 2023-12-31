#include "Vec3.h"
#include "Brick.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <limits>

struct Extents {
    int xMin = INT_MAX;
    int xMax = 0;
    int yMin = INT_MAX;
    int yMax = 0;
    int zMin = INT_MAX;
    int zMax = 0;

    void GrowToFit(Vec3 start, Vec3 end) {
        if (start.x < xMin)
            xMin = start.x;
        if (start.x > xMax)
            xMax = start.x;
        if (start.y < yMin)
            yMin = start.y;
        if (start.y > yMax)
            yMax = start.y;
        if (start.z < zMin)
            zMin = start.z;
        if (start.z > zMax)
            zMax = start.z;

        if (end.x < xMin)
            xMin = end.x;
        if (end.x > xMax)
            xMax = end.x;
        if (end.y < yMin)
            yMin = end.y;
        if (end.y > yMax)
            yMax = end.y;
        if (end.z < zMin)
            zMin = end.z;
        if (end.z > zMax)
            zMax = end.z;
    }
};

class Volume {
public:
    std::vector<std::vector<std::vector<Brick*>>> cells;

    Volume(const std::vector<Brick*>& bricks, const Extents& extents) {
        
        for (int z = extents.zMax; z >= extents.zMin; z--) {
            std::vector<std::vector<Brick*>> layer;
            for (int y = extents.yMin; y <= extents.yMax; y++) {
                std::vector<Brick*> row;
                for (int x = extents.xMin; x <= extents.xMax; x++) {
                    row.push_back(nullptr);
                }
                layer.push_back(row);
            }
            cells.push_back(layer);
        }        
    }
};

std::vector<Brick*> ParseInput(const std::string& fileName) {

    std::vector<Brick*> bricks;
    Extents extents;

    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    std::regex brickPattern(R"((\d+),(\d+),(\d+)~(\d+),(\d+),(\d+))");    
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        std::smatch match;
        std::regex_search(parsedLine, match, brickPattern);

        int x1 = std::stoi(match[1].str());
        int y1 = std::stoi(match[2].str());
        int z1 = std::stoi(match[3].str());
        int x2 = std::stoi(match[4].str());
        int y2 = std::stoi(match[5].str());
        int z2 = std::stoi(match[6].str());

        Vec3 start(x1, y1, z1);
        Vec3 end(x2, y2, z2);        
        Brick* brick = new Brick(start,end);
        bricks.push_back(brick);
        extents.GrowToFit(start, end);
    }

    return bricks;
}

void SettleBricks(std::vector<Brick*>& bricks) {
    
    std::vector<Brick*> movingBricks(bricks);
    while (movingBricks.size() > 0) {        
        for (auto brick : movingBricks) {
            if (brick->isPositionLocked)
                continue;
        }
    }
}

void DoPart1(std::vector<Brick*>& bricks) {
    long long candidateBrickCount = 0;

    SettleBricks(bricks);

    std::cout << "PART 1 ANSWER - How many single bricks could be disintegrated without disturbing the pile?: " << candidateBrickCount << "\n";
}

void DoPart2(std::vector<Brick*>& bricks) {
    // Not yet implemented
}

int main()
{
    std::cout << "Advent of Code - Day 21!\n";

    auto bricks = ParseInput("input.txt");
    DoPart1(bricks);
    DoPart2(bricks);
}
