#pragma once
#include "Terrain.h"
#include <sstream>
#include <iostream>
#include <vector>

struct Cell {
public:
    Terrain terrain;

    Cell(Terrain terrain) : terrain(terrain) {};
};

struct Grid {
public:
    short width;
    short height;

    Grid(const std::vector<std::string>& input) {
        for (const auto& line : input) {
            std::vector<Cell*> row;
            for (const auto& c : line) {
                switch (c) {
                case '#': row.push_back(new Cell(FOREST)); break;
                case '.': row.push_back(new Cell(PATH)); break;
                case '^': row.push_back(new Cell(NORTHSLOPE)); break;
                case '>': row.push_back(new Cell(EASTSLOPE)); break;
                case 'v': row.push_back(new Cell(SOUTHSLOPE)); break;
                case '<': row.push_back(new Cell(WESTSLOPE)); break;
                default: throw "Unrecognized input.";
                }
            }
            content.push_back(row);
        }

        width = static_cast<short>(content[0].size());
        height = static_cast<short>(content.size());
    }

    Cell* GetAt(short x, short y) const {
        return content[y][x];
    }
    Cell* GetAt(Vec2 position) const {
        return GetAt(position.x, position.y);
    }

    bool IsWithinBounds(Vec2 position) const {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

    void PrintGrid() {
        std::cout << "---Grid---\n";
        for (unsigned char y = 0; y < height; y++) {
            for (unsigned char x = 0; x < width; x++) {
                std::cout << TerrainToChar(GetAt(x, y)->terrain);
            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    }

private:
    std::vector<std::vector<Cell*>> content;
};