#ifndef GRID_CPP
#define GRID_CPP

#include "Vec2.cpp"
#include "Cell.cpp""
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
				Cell* newCell = new Cell(character);
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

	std::vector<Cell*>& GetRow(int y) {
		return content[y];
	}

	std::vector<Cell*> GetColumn(int x) {
		std::vector<Cell*> column;
		for (int y = 0; y < GetHeight(); y++) {
			auto row = content[y];
			column.push_back(row[x]);
		}
		return column;
	}

	void InsertColumnBefore(int x) {
		for (int y = 0; y < GetHeight(); y++) {
			auto& row = content[y];
			auto newCell = new Cell('.');
			auto it = row.begin();
			row.insert(row.begin() + x, newCell);
		}
	}

	void InsertRowBefore(int y) {
		std::vector<Cell*> newRow;
		for (int x = 0; x < GetWidth(); x++) {
			auto newCell = new Cell('.');
			newRow.push_back(newCell);
		}
		auto it = content.begin();
		content.insert(it + y, newRow);
	}

	void RefreshCellPositionData() {
		for (int y = 0; y < GetHeight(); y++) {
			for (int x = 0; x < GetWidth(); x++) {
				content[y][x]->position = Vec2(x, y);
			}
		}
	}

	void PrintGrid() {
		std::cout << "---Grid--------------------\n";
		for (int y = 0; y < GetHeight(); y++) {
			for (int x = 0; x < GetWidth(); x++) {
				auto cell = GetCellAt(Vec2(x, y));				
				std::cout << cell->content;				
			}
			std::cout << "\n";
		}
		std::cout << "---------------------------\n";
	}

private:
	std::vector<std::vector<Cell*>> content;
};

#endif