#pragma once

#include "Vec2.h"
#include <vector>
#include <iostream>
#include <algorithm>

template <typename T>
class Grid {
public:
    std::vector<std::vector<std::size_t>> content;
    std::size_t width;
    std::size_t height;

    Grid(const std::vector<std::string>& input) {
        width = input[0].size();
        height = input.size();
        for (std::size_t y = 0; y < height; y++) {
            std::vector<std::size_t> row;
            for (std::size_t x = 0; x < width; x++) {
                std::size_t value = input[y][x] - '0';
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
        for (std::size_t y = 0; y < height; y++) {
            for (std::size_t x = 0; x < width; x++) {
                auto value = Get(Vec2(x, y));
                std::cout << std::to_string(value);
            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    }
};

std::vector<Vec2> GetValidNeighboursPositions(const Node* node, int minSteps, int maxSteps, const Grid<size_t>& grid) {
        
    // Since the crucible can't turn around, we eliminate "backwards" direction
    Vec2 currentDirection = Vec2(0, 0);
    if (node->previousNode)
        currentDirection = node->position - node->previousNode->position;

    std::vector<Vec2> possibleDirections = GetPossibleNextDirections(currentDirection);
    std::vector<Vec2> possibleNeighbours;
    possibleNeighbours.reserve(possibleDirections.size());
    for (auto possibleDirection : possibleDirections)
        possibleNeighbours.push_back(possibleDirection + node->position);

    // Eliminate positions that are outside grid bounds        
    possibleNeighbours.erase(
        std::remove_if(possibleNeighbours.begin(), possibleNeighbours.end(), [&grid](Vec2 position) {
        return !grid.IsWithinBounds(position);
        }), possibleNeighbours.end());

    // Must travel at least minSteps in the same direction
    if (node->steps > 0 && node->steps < minSteps) {
        possibleNeighbours.erase(
            std::remove_if(possibleNeighbours.begin(), possibleNeighbours.end(), [&node, &currentDirection](Vec2 position) {
            Vec2 nextDirection = position - node->position;
            return nextDirection != currentDirection;
            }), possibleNeighbours.end());        
    }

    // Cannot travel more than maxSteps in the same direction
    if (node->steps >= maxSteps) {        
        possibleNeighbours.erase(
            std::remove_if(possibleNeighbours.begin(), possibleNeighbours.end(), [&grid, &node](Vec2 position) {
            if (node->previousNode) {
                Vec2 prevDirection = node->position - node->previousNode->position;
                Vec2 nextDirection = position - node->position;
                return nextDirection == prevDirection;
            }
            else
                return false;
            }), possibleNeighbours.end());         
    }

    return possibleNeighbours;
}