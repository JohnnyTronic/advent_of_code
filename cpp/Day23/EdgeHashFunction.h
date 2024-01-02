#pragma once
#include "Node.h"
#include <cstddef>

struct EdgeHashFunction {
    std::size_t operator()(const Edge* edge) const {
        return static_cast<std::size_t>(edge->nodeA->position.x) + (static_cast<std::size_t>(edge->nodeA->position.y) << 16) + (static_cast<std::size_t>(edge->nodeB->position.x) << 32) + (static_cast<std::size_t>(edge->nodeB->position.y) << 48);
    }
};