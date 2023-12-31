#pragma once

#include "Vec3.h"
#include <vector>
#include <cmath>

class Brick {
public:
	char name;
	Vec3 start;
	Vec3 end;
	std::vector<Vec3> interpolatedPositions;
	bool isPositionLocked = false;
	std::vector<Brick*> contactingBricksBelow;
	std::vector<Brick*> contactingBricksAbove;
	int zMin;

	Brick(char name, Vec3 start, Vec3 end) : name(name), start(start), end(end) {
		interpolatedPositions = InterpolateCellPositions(start, end);
	};

	std::vector<Vec3> InterpolateCellPositions(Vec3 start, Vec3 end) {
		zMin = INT_MAX;
		auto delta = end - start;
		double length = std::sqrt((delta.x * delta.x) + (delta.y * delta.y) + (delta.z * delta.z));
		Vec3 unitDelta = delta * (1.0 / length);

		interpolatedPositions.clear();
		for (int i = 0; i <= length; i++) {
			Vec3 pos = start + unitDelta * i;
			interpolatedPositions.push_back(pos);
			if (pos.z < zMin)
				zMin = pos.z;
		}

		return interpolatedPositions;
	}

	void MoveDown(int steps) {
		Vec3 downStep(0, 0, -steps);
		start = start + downStep;
		end = end + downStep;
		interpolatedPositions = InterpolateCellPositions(start, end);
	}

	bool OverlapsPosition(Vec3 position) {
		for (auto& interpolated : interpolatedPositions) {
			if (interpolated == position)
				return true;
		}

		return false;
	}
};