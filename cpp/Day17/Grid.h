#pragma once

#include "Vec2.h"
#include <vector>
#include <iostream>
#include <algorithm>

template <typename T>
class Grid {
public:
    std::vector<std::vector<int>> content;
    int width;
    int height;

    Grid(const std::vector<std::string>& input) {
        width = input[0].size();
        height = input.size();
        for (int y = 0; y < height; y++) {
            std::vector<int> row;
            for (int x = 0; x < width; x++) {
                int value = input[y][x] - '0';
                row.push_back(value);
            }
            content.push_back(row);
        }
    }

    T Get(Vec2 position) const {
        return content[position.y][position.x];
    }

    bool IsWithinBounds(const Vec2& position) const {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

    void PrintGrid() const {
        std::cout << "---Grid---\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto value = Get(Vec2(x, y));
                std::cout << std::to_string(value);
            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    }
};

std::vector<Vec2> GetValidNeighboursPositions(const Node* node, const Grid<int>& grid) {

    // Since the crucible can't turn around, we eliminate "backwards" direction
    Vec2 currentDirection = Vec2(0, 0);
    if (node->previousNode)
        currentDirection = node->position - node->previousNode->position;
    std::vector<Vec2> possibleDirections = GetPossibleNextDirections(currentDirection);
    std::vector<Vec2> nonBackwardNeighbours;
    for (auto possibleDirection : possibleDirections)
        nonBackwardNeighbours.push_back(possibleDirection + node->position);

    // Eliminate positions that are outside grid bounds    
    std::vector<Vec2> inBoundsNeighbours;
    std::copy_if(nonBackwardNeighbours.begin(), nonBackwardNeighbours.end(), std::back_inserter(inBoundsNeighbours), [&grid](Vec2 position) {
        return grid.IsWithinBounds(position);
        });

    // Cannot travel 4 steps in the same direction
    if (node->steps >= 3) {
        std::vector<Vec2> stepLimitedNeighbours;
        std::copy_if(inBoundsNeighbours.begin(), inBoundsNeighbours.end(), std::back_inserter(stepLimitedNeighbours), [&grid, &node](Vec2 position) {
            if (node->previousNode) {
                Vec2 prevDirection = node->position - node->previousNode->position;
                Vec2 nextDirection = position - node->position;
                return nextDirection != prevDirection;
            }
            else
                return true;
            });         
        return stepLimitedNeighbours;
    }

    return inBoundsNeighbours;
}