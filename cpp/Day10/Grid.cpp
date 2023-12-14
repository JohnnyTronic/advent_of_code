#ifndef GRID_CPP
#define GRID_CPP

#include "Direction.h"
#include <vector>
#include <sstream>
#include "Vec2.cpp"

class Grid {
public:
	Grid(std::vector<std::string> rowStrings) {
		for (auto& row : rowStrings) {
			std::vector<char> rowChars(row.begin(), row.end());
			content.push_back(rowChars);
		}
	}

	int GetWidth() {
		return content[0].size();
	}

	int GetHeight() {
		return content.size();
	}

	char GetCharAt(const Vec2& coord) {
		return content[coord.y][coord.x];
	}

	std::vector<Vec2> GetNeighbourCoords(const Vec2& startingCoord) {
		std::vector<Vec2> neighbours;

		auto up = startingCoord + Vec2(0, -1);
		if (up.y >= 0)
			neighbours.push_back(up);

		auto right = startingCoord + Vec2(1, 0);
		if (right.x < GetWidth())
			neighbours.push_back(right);

		auto down = startingCoord + Vec2(0, 1);
		if (down.y < GetHeight())
			neighbours.push_back(down);

		auto left = startingCoord + Vec2(-1, 0);
		if (left.x >= 0)
			neighbours.push_back(left);

		return neighbours;
	}

private:
	std::vector<std::vector<char>> content;
};

#endif