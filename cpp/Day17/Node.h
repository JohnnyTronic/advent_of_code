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

    std::string Hash() {
        Vec2 direction(0, 0);
        if (previousNode)
            direction = Vec2(previousNode->position.x - position.x, previousNode->position.y - position.y);
        int directionInt = 0;
        if (direction == NORTH) directionInt = 1;
        if (direction == SOUTH) directionInt = 2;
        if (direction == EAST) directionInt = 3;
        if (direction == WEST) directionInt = 4;

        //std::string stringHash = std::to_string(position.x) + std::to_string(position.y) + std::to_string(steps) + std::to_string(costSoFar);
        std::string stringHash = std::to_string(directionInt) + std::to_string(steps) + std::to_string(position.x) + std::to_string(position.y);
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(10) << stringHash;
        return ss.str();
    }
};