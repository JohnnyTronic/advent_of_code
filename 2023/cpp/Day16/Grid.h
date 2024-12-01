#pragma once

#ifndef GRID_H
#define GRID_H

#include "Vec2.h"
#include "GridCell.h"
#include <iostream>
#include <vector>
#include <sstream>

class Grid {
public:
    std::vector<std::vector<GridCell*>> gridCells;
    size_t width;
    size_t height;

    Grid(std::vector<std::string> input);

    GridCell* GetCellAt(Vec2 pos) {
        return gridCells[pos.y][pos.x];
    };

    void Reset() {
        for (int y = 0; y < this->height; y++) {
            for (int x = 0; x < width; x++) {
                auto cell = GetCellAt(Vec2(x, y));
                cell->Reset();
            }
        }
    };

    long long FireBeam(Vec2 position, Vec2 direction);
    long long CountEnergizedCells() {
        long long energizedCount = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                GridCell* cell = GetCellAt(Vec2(x, y));
                if (cell->isEnergized)
                    energizedCount++;
            }
        }

        return energizedCount;
    };

    void PrintGrid() {
        std::cout << "---Grid---\n";
        for (auto& row : gridCells) {
            for (auto cell : row) {
                std::cout << cell->content;
            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    };

    void PrintEnergizedGrid() {
        std::cout << "---EnergizedGrid---\n";
        for (auto& row : gridCells) {
            for (auto cell : row) {
                if (cell->isEnergized) {
                    std::cout << "#";
                }
                else {
                    std::cout << '.';
                }

            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    };
};
#endif