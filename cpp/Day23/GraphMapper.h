#pragma once
#include "Vec2.h"
#include "Grid.h"
#include "NodeGraph.h"
#include <vector>

struct GraphMapper {
    Vec2 position;
    Vec2 previousPosition;
    Node* previousNode;  
    int distance;

    GraphMapper(Vec2 position, Vec2 previousPosition, Node* previousNode, int distance = 0) : position(position), previousPosition(previousPosition), previousNode(previousNode), distance(distance) {};

    std::vector<Vec2> FindNextPositions(const Grid& grid) {
        std::vector<Vec2> offsets{ NORTH, EAST, SOUTH, WEST };
        std::vector<Vec2> nextPositions;
        for (auto offset : offsets) {
            Vec2 candidatePosition = position + offset;

            if (!grid.IsWithinBounds(candidatePosition))
                continue;

            if (candidatePosition == previousPosition)
                continue;

            if (grid.GetAt(candidatePosition)->terrain == FOREST)
                continue;

            nextPositions.push_back(candidatePosition);
        }

        return nextPositions;
    }

    void MoveTo(Vec2 nextPosition) {
        previousPosition = position;
        position = nextPosition;
        distance++;
    }
};