#include "Vec2.h"
#include "GridCell.h"
#include "Beam.h"
#include "Grid.h"
#include <iostream>
#include <vector>
#include <set>

Grid::Grid(std::vector<std::string> input) {
	for (auto& string : input) {
		std::vector<GridCell*> row;
		for (auto& c : string) {
			GridCell* cell = new GridCell(c);
			row.push_back(cell);
		}
		gridCells.push_back(row);
	}

	height = gridCells.size();
	width = gridCells[0].size();
}

long long Grid::FireBeam(Vec2 position, Vec2 direction) {
	std::set<Beam*> beams;
	Beam* firstBeam = new Beam(position, direction);
	GetCellAt(firstBeam->position)->isEnergized = true;
	beams.insert(firstBeam);

	std::set<Beam*> endedBeams;
	while (beams.size() > 0) {

		for (auto beam : beams) {
			bool beamEnded = beam->Tick(*this, beams);
			if (beamEnded) {
				endedBeams.insert(beam);
			}
		}

		for (auto beam : endedBeams) {
			beams.erase(beam);
			delete beam;
		}
		endedBeams.clear();
	}

	//PrintEnergizedGrid();
	long long totalEnergizedCells = CountEnergizedCells();
	return totalEnergizedCells;
}
