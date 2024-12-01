#ifndef GRID_CPP
#define GRID_CPP

#include "Vec2.cpp"
#include "Cell.cpp""
#include <vector>
#include <sstream>

class SpaceLine {
public:
	long expandedSize;
	SpaceLine(long expandedSize) : expandedSize(expandedSize) {}
};

class Grid {
public:
	std::vector<Cell*> galaxies;
	std::vector<SpaceLine> rows;
	std::vector<SpaceLine> columns;

	Grid(std::vector<std::string> rowStrings) {

		// Record galaxies
		for (int y = 0; y < rowStrings.size(); y++) {
			std::string row = rowStrings[y];
			for (int x = 0; x < row.size(); x++) {
				char character = row[x];
				if (character == '#') {
					Cell* newCell = new Cell(character, Vec2(x,y));
					galaxies.push_back(newCell);
				}
			}
		}

		// Record rows
		for (int y = 0; y < rowStrings.size(); y++) {
			SpaceLine newRow(1);
			rows.push_back(newRow);
		}

		// Record columns
		for (int x = 0; x < rowStrings[0].size(); x++) {
			SpaceLine newColumn(1);
			columns.push_back(newColumn);
		}
	}

	int GetWidth() {
		return columns.size();
	}

	int GetHeight() {
		return rows.size();
	}

	/*Cell* GetCellAt(const Vec2& coord) {
		for (auto galaxy : galaxies) {
			if (galaxy->position == coord)
				return galaxy;
		}
		return nullptr;
	}

	std::vector<Cell*>& GetRow(int y) {
		return rows[y];
	}

	std::vector<Cell*> GetColumn(int x) {
		std::vector<Cell*> column;
		for (int y = 0; y < GetHeight(); y++) {
			auto row = content[y];
			column.push_back(row[x]);
		}
		return column;
	}*/

	void ExpandColumn(int x, long times) {
		columns[x].expandedSize = times;		
	}

	void ExpandRow(int y, long times) {
		rows[y].expandedSize = times;		
	}

	/*void PrintGrid() {
		std::cout << "---Grid--------------------\n";
		for (int baseY = 0; baseY < rows.size(); baseY++) {
			for (int baseX = 0; baseX < columns.size(); baseX++) {
				auto cell = GetCellAt(Vec2(baseX, baseY));				
				std::cout << cell->content;				
			}
			std::cout << "\n";
		}
		std::cout << "---------------------------\n";
	}*/

};

#endif