#pragma once

class GridCell {
public:
    char content;
    bool isEnergized = false;
    bool beamedNorth = false;
    bool beamedSouth = false;
    bool beamedEast = false;
    bool beamedWest = false;
    GridCell(char content) : content(content) {}

    void Reset() {
        isEnergized = false;
        beamedNorth = false;
        beamedSouth = false;
        beamedEast = false;
        beamedWest = false;
    }
};