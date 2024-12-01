#pragma once
#include "Node.h"
#include <cstddef>

struct NodeHashFunction {
    std::size_t operator()(const Node* node) const {
        return static_cast<std::size_t>(node->position.x) + (static_cast<std::size_t>(node->position.y) << 16);
    }
};