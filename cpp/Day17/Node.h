#pragma once

#include "Vec2.h"
#include <sstream>
#include <iomanip>

struct Node {
public:
    Vec2 position{};
    //Vec2 previousPosition;
    Node* previousNode;
    int steps = 0;
    long costSoFar{};

    Node(Vec2 position, Node* previousNode, int steps, long costSoFar) : position(position), previousNode(previousNode), steps(steps), costSoFar(costSoFar) {}

    bool operator==(const Node& other) const { return position == other.position && &previousNode == &other.previousNode && steps == other.steps && costSoFar == other.costSoFar; }
    bool operator<(const Node& other) const { return costSoFar < other.costSoFar; }

    long long Hash() {
        Vec2 direction(0, 0);
        if (previousNode)
            direction = Vec2(previousNode->position.x - position.x, previousNode->position.y - position.y);
        int directionInt = 0;
        if (direction == NORTH) directionInt = 1;
        if (direction == SOUTH) directionInt = 2;
        if (direction == EAST) directionInt = 3;
        if (direction == WEST) directionInt = 4;
        
        return directionInt + (steps << 4) + (position.x << 8) + (position.y << 12) + (costSoFar << 16);       
    }
};