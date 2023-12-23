#pragma once

#include "Vec2.h"
#include <vector>
#include <iostream>

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