#pragma once
#include "Vec2.h"
#include <unordered_set>

class GridWalker {
public:
    Vec2 position;
    int steps;
    std::unordered_set<Vec2, Vec2::HashFunction> visited;

    GridWalker(Vec2 position, int steps, std::unordered_set<Vec2, Vec2::HashFunction> visited) : position(position), steps(steps), visited(visited) {
        this->visited.insert(position);
    }

    void MoveTo(Vec2 nextPosition) {
        position = nextPosition;
        steps++;
        visited.insert(nextPosition);
    }

    GridWalker* Clone() {
        GridWalker* newGridWalker = new GridWalker(position, steps, visited);
        return newGridWalker;
    }
};