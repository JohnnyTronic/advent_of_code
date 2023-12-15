#ifndef GRID_CPP
#define GRID_CPP

#include "Direction.h"
#include "Vec2.cpp"
#include "Cell.cpp"
#include <vector>
#include <sstream>

class Grid {
public:
	Grid(std::vector<std::string> rowStrings) {
		for (int y = 0; y < rowStrings.size(); y++) {
			std::string row = rowStrings[y];
			std::vector<Cell*> rowCells;
			for (int x = 0; x < row.size(); x++) {
				char character = row[x];
				Cell* newCell = new Cell(Vec2(x, y), character, false);
				rowCells.push_back(newCell);
			}
			content.push_back(rowCells);
		}
	}

	int GetWidth() {
		return content[0].size();
	}

	int GetHeight() {
		return content.size();
	}

	Cell* GetCellAt(const Vec2& coord) {
		return content[coord.y][coord.x];
	}

	std::vector<Cell*> GetNeighbourCells(const Vec2& startingCoord) {
		std::vector<Cell*> neighbourCells;

		auto up = startingCoord + Vec2(0, -1);
		if (up.y >= 0) {
			auto upNeighbour = GetCellAt(up);
			neighbourCells.push_back(upNeighbour);
		}

		auto right = startingCoord + Vec2(1, 0);
		if (right.x < GetWidth()) {
			auto rightNeighbour = GetCellAt(right);
			neighbourCells.push_back(rightNeighbour);
		}

		auto down = startingCoord + Vec2(0, 1);
		if (down.y < GetHeight()) {
			auto downNeighbour = GetCellAt(down);
			neighbourCells.push_back(downNeighbour);
		}

		auto left = startingCoord + Vec2(-1, 0);
		if (left.x >= 0) {
			auto leftNeighbour = GetCellAt(left);
			neighbourCells.push_back(leftNeighbour);
		}

		return neighbourCells;
	}

private:
	std::vector<std::vector<Cell*>> content;
};

#endif