#pragma once

#include "Node.h"

struct NodeCostCompare {
    bool operator()(const Node* a, const Node* b) {
        return a->costSoFar > b->costSoFar;
    }
};