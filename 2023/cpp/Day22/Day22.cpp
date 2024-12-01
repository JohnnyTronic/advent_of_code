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
#include <algorithm>

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
    std::unordered_map<std::size_t, Brick*> positionHashedBricks;

    Volume(const std::vector<Brick*>& bricks) {
        for (auto brick : bricks) {
            for (auto interpolatedPosition : brick->interpolatedPositions) {
                auto hash = interpolatedPosition.Hash();
                if (positionHashedBricks.contains(hash))
                    throw "Initial hash error.";
                positionHashedBricks[hash] = brick;
            }
        }
    }

    Brick* GetAt(Vec3 position) {
        auto hash = position.Hash();
        if (positionHashedBricks.contains(hash)) {
            return positionHashedBricks[hash];
        }

        return nullptr;
    }

    void ClearAt(Vec3 position) {
        auto hash = position.Hash();
        positionHashedBricks.erase(hash);
    }

    void SetAt(Vec3 position, Brick* brick) {
        auto hash = position.Hash();
        positionHashedBricks[hash] = brick;
    }

    std::unordered_set<Brick*> GetBricksBelowBrick(Brick* targetBrick) {
        std::unordered_set<Brick*> bricksBelow;

        auto& interpolatedPositions = targetBrick->interpolatedPositions;
        for (auto& position : interpolatedPositions) {
            Vec3 belowPosition = position + Vec3(0, 0, -1);
            auto belowHash = belowPosition.Hash();
            auto brickBelow = GetAt(belowHash);
            if (brickBelow != nullptr && brickBelow != targetBrick && !bricksBelow.contains(brickBelow)) {
                bricksBelow.insert(brickBelow);
            }
        }

        return bricksBelow;
    }
};

std::vector<Brick*> ParseInput(const std::string& fileName) {

    int nextBrickID = 'A';
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
        Brick* brick = new Brick(nextBrickID++, start,end);
        if (brick->zMin == 1)
            brick->isPositionLocked = true;
        bricks.push_back(brick);
        extents.GrowToFit(start, end);
    }

    return bricks;
}

void MoveBrickDownInVolume(Brick* brick, Volume& volume) {
    for (auto& pos : brick->interpolatedPositions) {
        volume.ClearAt(pos);
    }
    brick->MoveDown(1);
    for (auto& pos : brick->interpolatedPositions) {
        volume.SetAt(pos, brick);
    }
}

void SettleBricks(std::vector<Brick*>& bricks, Volume& volume) {
    
    std::deque<Brick*> movingBricks;
    for (auto brick : bricks)
        movingBricks.push_back(brick);
    movingBricks.erase(std::remove_if(movingBricks.begin(), movingBricks.end(), [](auto testBrick) {return testBrick->isPositionLocked; }));

    while (movingBricks.size() > 0) { 
        Brick* brick = movingBricks.front();
        movingBricks.pop_front();
               
        auto bricksBelow = volume.GetBricksBelowBrick(brick);
        if (bricksBelow.size() == 0) {
            MoveBrickDownInVolume(brick,volume);
            if (brick->zMin == 1) {
                brick->isPositionLocked = true;
                continue;
            }
            else {
                movingBricks.push_back(brick);
            }                
        }
        else {
            bool allBricksBelowLocked = true;
            for (auto brickBelow : bricksBelow) {
                if (brickBelow->isPositionLocked == false) {
                    allBricksBelowLocked = false;
                    break;
                }
            }
            if (allBricksBelowLocked) {
                brick->isPositionLocked = true;
                continue;                   
            }
            else {
                movingBricks.push_back(brick);
            }
        }        
    }
}

void DoPart1(std::string inputFile) {

    auto bricks = ParseInput(inputFile);
    Volume volume(bricks);    
    SettleBricks(bricks, volume);

    std::unordered_set<Brick*> criticalBricks;
    std::unordered_set<Brick*> redundantBricks;
    for (auto brick : bricks) {
        brick->contactingBricksBelow.clear();
        brick->contactingBricksAbove.clear();
    }

    for (auto focusBrick : bricks) {
        auto bricksBelow = volume.GetBricksBelowBrick(focusBrick);
        for (auto brickBelow : bricksBelow) {
            brickBelow->contactingBricksAbove.push_back(focusBrick);
            focusBrick->contactingBricksBelow.push_back(brickBelow);
        }

        if (bricksBelow.size() == 1) {
            criticalBricks.insert(*bricksBelow.begin());
        }

        if (bricksBelow.size() > 1) {
            for (auto brickBelow : bricksBelow) {
                redundantBricks.insert(brickBelow);
            }            
        }          
    }

    std::erase_if(redundantBricks, [&](auto testBrick) {return criticalBricks.contains(testBrick); });
   
    for (auto brick : bricks) {
        if (brick->contactingBricksAbove.size() == 0)
            redundantBricks.insert(brick);
    }

    std::cout << "PART 1 ANSWER - How many single bricks could be disintegrated without disturbing the pile?: " << redundantBricks.size() << "\n";
}

bool AreSupportBricksIncludedInGroup(Brick* focusBrick, std::unordered_set<Brick*> fallingBricks) {
    bool allContained = true;
    for (auto belowBrick : focusBrick->contactingBricksBelow) {
        if (!fallingBricks.contains(belowBrick)) {
            allContained = false;
            break;
        }
    }
    return allContained;
}

long long CalculateChainReactionForBrick(Brick* focusBrick, std::unordered_set<Brick*>& fallingBricks) {

   // if (focusBrick->chainReactionScore != -1)
     //   return focusBrick->chainReactionScore;

    long long chainReactionCount = 0;
    
    std::unordered_set<Brick*> newlyFallingBricks;
    for (auto brickAbove : focusBrick->contactingBricksAbove) {
        if (AreSupportBricksIncludedInGroup(brickAbove, fallingBricks)) {
            fallingBricks.insert(brickAbove);
            chainReactionCount++;
            newlyFallingBricks.insert(brickAbove);
        }
    }   

    for (auto newlyFallingBrick : newlyFallingBricks) {
        chainReactionCount += CalculateChainReactionForBrick(newlyFallingBrick, fallingBricks);
    }

    focusBrick->chainReactionScore = chainReactionCount;
    return chainReactionCount;
}

int CalculateDepth(Brick* brick) {
    int depth = 0;
    
    if (brick->contactingBricksAbove.size() == 0) {
        brick->depth = 0;
        return brick->depth;
    }

    int maxDepth = 0;
    for (auto brickAbove : brick->contactingBricksAbove) {
        int depth = -1;
        if (brickAbove->depth == -1)
            depth = CalculateDepth(brickAbove);
        else
            depth = brickAbove->depth;

        if (depth + 1 > maxDepth)
            maxDepth = depth + 1;
    }

    brick->depth = maxDepth;
    return brick->depth;
}

void DoPart2(std::string inputFile) {
    auto bricks = ParseInput(inputFile);
    Volume volume(bricks);
    SettleBricks(bricks, volume);

    // Connect bricks to their above/below neighbours
    for (auto focusBrick : bricks) {
        auto bricksBelow = volume.GetBricksBelowBrick(focusBrick);
        for (auto brickBelow : bricksBelow) {
            brickBelow->contactingBricksAbove.push_back(focusBrick);
            focusBrick->contactingBricksBelow.push_back(brickBelow);
        }
    }

    // Calculate the depth of each brick
    for (auto brick : bricks) {
        CalculateDepth(brick);
    }

    // Sort bricks by depth, least deep first
    std::sort(bricks.begin(), bricks.end(), [](Brick* brickA, Brick* brickB) {return brickA->depth < brickB->depth; });

    // For each brick, calculate how many bricks would start falling if the focus brick was disintegrated
    long long sumOfChainReactions = 0;
    long brickIndex = 0;
    for (auto brick : bricks) {
        std::cout << "Calculating chain score for brick: " << brick->name << ", brickIndex: " << brickIndex << "\n";
        std::unordered_set<Brick*> fallingBricks{ brick };
        CalculateChainReactionForBrick(brick, fallingBricks);
        sumOfChainReactions += fallingBricks.size() - 1;
        brickIndex++;
    }
      
    std::cout << "PART 2 ANSWER - Sum of chain reactions: " << sumOfChainReactions << "\n";
}

int main()
{
    std::cout << "Advent of Code - Day 22!\n";

    std::string inputFile = "input.txt";
    DoPart1(inputFile);
    DoPart2(inputFile);
}
