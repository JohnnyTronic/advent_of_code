#pragma once

#include "Vec2.h"
#include "Grid.h"
#include <set>

class Beam {
public:
    Vec2 position;
    Vec2 direction;
    Beam(Vec2 position, Vec2 direction) : position(position), direction(direction) {}

    bool Tick(Grid& grid, std::set<Beam*>& beams) {

        auto cell = grid.GetCellAt(position);
        cell->isEnergized = true;

        // Have we visited this cell before while travelling this direction?
        if (direction == Vec2(0, -1) && cell->beamedNorth)
            return true;
        if (direction == Vec2(0, 1) && cell->beamedSouth)
            return true;
        if (direction == Vec2(1, 0) && cell->beamedEast)
            return true;
        if (direction == Vec2(-1, 0) && cell->beamedWest)
            return true;

        if (direction == Vec2(1, 0)) {
            BeamRight(grid, beams);
        }
        else if (direction == Vec2(-1, 0)) {
            BeamLeft(grid, beams);
        }
        else if (direction == Vec2(0, -1)) {
            BeamUp(grid, beams);
        }
        else if (direction == Vec2(0, 1)) {
            BeamDown(grid, beams);
        }
        else {
            throw "Unrecognized direction.";
        }

        Vec2 nextPosition = position + direction;
        if (nextPosition.x < 0 || nextPosition.x >= grid.width || nextPosition.y < 0 || nextPosition.y >= grid.height) {
            // This beam ends
            return true;
        }
        else {
            // Move a step forward in the direction we're heading
            position = position + direction;
        }

        return false;
    }

private:
    void BeamRight(Grid& grid, std::set<Beam*>& beams) {

        auto cell = grid.GetCellAt(position);
        cell->beamedEast = true;
        switch (cell->content) {
        case '.':
        case '-': {
            //BeamRight(start + Vec2(1, 0), grid);
            break;
        }
        case '/': {
            direction = Vec2(0, -1);
            break;
        }
        case '\\': {
            direction = Vec2(0, 1);
            break;
        }
        case '|': {
            direction = Vec2(0, -1);
            Beam* splitBeam = new Beam(position, Vec2(0, 1));
            beams.insert(splitBeam);
            break;
        }
        default: {
            throw "Unexpected grid entry: " + cell->content;
        }
        }
    }

    void BeamLeft(Grid& grid, std::set<Beam*>& beams) {
        auto cell = grid.GetCellAt(position);
        cell->beamedWest = true;
        switch (cell->content) {
        case '-':
        case '.': {
            break;
        }
        case '/': {
            direction = Vec2(0, 1);
            break;
        }
        case '\\': {
            direction = Vec2(0, -1);
            break;
        }
        case '|': {
            direction = Vec2(0, -1);
            Beam* splitBeam = new Beam(position, Vec2(0, 1));
            beams.insert(splitBeam);
            break;
        }
        default: {
            throw "Unexpected grid entry: " + cell->content;
        }
        }
    }

    void BeamUp(Grid& grid, std::set<Beam*>& beams) {
        auto cell = grid.GetCellAt(position);
        cell->beamedNorth = true;
        switch (cell->content) {
        case '.':
        case '|': {
            break;
        }
        case '/': {
            direction = Vec2(1, 0);
            break;
        }
        case '\\': {
            direction = Vec2(-1, 0);
            break;
        }
        case '-': {
            direction = Vec2(-1, 0);
            Beam* splitBeam = new Beam(position, Vec2(1, 0));
            beams.insert(splitBeam);
            break;
        }
        default: {
            throw "Unexpected grid entry: " + cell->content;
        }
        }
    }

    void BeamDown(Grid& grid, std::set<Beam*>& beams) {
        auto cell = grid.GetCellAt(position);
        cell->beamedSouth = true;
        switch (cell->content) {
        case '.':
        case '|': {
            break;
        }
        case '/': {
            direction = Vec2(-1, 0);
            break;
        }
        case '\\': {
            direction = Vec2(1, 0);
            break;
        }
        case '-': {
            direction = Vec2(-1, 0);
            Beam* splitBeam = new Beam(position, Vec2(1, 0));
            beams.insert(splitBeam);
            break;
        }
        default: {
            throw "Unexpected grid entry: " + cell->content;
        }
        }
    }
};