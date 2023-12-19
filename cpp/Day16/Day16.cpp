#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <map>

class Vec2 {
public:
    int x;
    int y;
    Vec2(int x, int y) : x(x), y(y) {};
};

static Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

static bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return (lhs.x == rhs.x && lhs.y == rhs.y);
}

class GridCell {
public:
    char content;
    bool isEnergized = false;
    bool beamedNorth = false;
    bool beamedSouth = false;
    bool beamedEast = false;
    bool beamedWest = false;
    GridCell(char content) : content(content) {}
};

class Grid {
public:
    std::vector<std::vector<GridCell*>> gridCells;
    int width;
    int height;   

    Grid(std::vector<std::string> input) {
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

    GridCell* GetCellAt(Vec2 pos) {
        return gridCells[pos.y][pos.x];
    }

    void PrintGrid() {
        std::cout << "---Grid---\n";
        for (auto& row : gridCells) {
            for (auto cell : row) {
                std::cout << cell->content;
            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    }

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
    }
};

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

long long CountEnergizedCells(Grid& grid) {
    long long energizedCount = 0;
    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            GridCell* cell = grid.GetCellAt(Vec2(x, y));
            if (cell->isEnergized)
                energizedCount++;
        }
    }

    return energizedCount;
}

int main()
{
    std::cout << "Advent of Code - Day 16!\n";

    std::ifstream ifs("input.txt");
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    Grid grid(parsedLines);
    grid.PrintGrid();

    std::set<Beam*> beams;
    Beam* firstBeam = new Beam(Vec2(0, 0), Vec2(1, 0));
    grid.GetCellAt(firstBeam->position)->isEnergized = true;
    beams.insert(firstBeam);

    std::set<Beam*> endedBeams;
    while (beams.size() > 0) {
        
        for (auto beam : beams) {           
            bool beamEnded = beam->Tick(grid, beams);
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
    
    grid.PrintEnergizedGrid();
    long long totalEnergizedCells = CountEnergizedCells(grid);

    std::cout << "PART 1 ANSWER - Total count of energized cells: " << totalEnergizedCells << "\n";
    


    return 0;
}