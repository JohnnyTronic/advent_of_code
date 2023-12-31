#include "Vec2.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
#include <vector>
#include <map>
#include <unordered_set>

enum CellContent {
    START,
    GARDEN,
    ROCK
};

CellContent GetCellContentForChar(const char& c) {
    switch (c) {
    case 'S': return START;
    case '.': return GARDEN;
    case '#': return ROCK;
    default: throw "Unrecognized cell char: " + c;
    }
}

class Cell {
public:
    CellContent content;
    bool isReachable = false;

    Cell(CellContent content) : content(content) {};
};

class Grid {
public:
    int width;
    int height;
    Vec2 startingPosition; 
    std::unordered_set<std::size_t> visitedCandidates;
    long long totalReachableCells = 0;

    Grid(const std::vector<std::string>& inputStrings) {
        int y = 0;
        for (const auto& line : inputStrings) {
            std::vector<Cell> contentRow;        
            int x = 0;
            for (const auto& c : line) {
                Cell cell(GetCellContentForChar(c));
                contentRow.push_back(cell);     
                if (c == 'S') {
                    startingPosition = Vec2(x, y);
                }
                x++;
            }
            content.push_back(contentRow);          
            y++;
        }

        width = content[0].size();
        height = content.size();
    }

    Cell& GetCell(Vec2& position) {
        return content[position.y][position.x];
    }

    bool IsWithinBounds(Vec2& position) {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

private:
    std::vector<std::vector<Cell>> content;
};

Grid ParseInput(const std::string& fileName) {

    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    std::regex wholeModulePattern(R"((.+)\s->\s(.+))");
    std::regex moduleNamePattern(R"(([%&])?(\w+))");
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    Grid grid(parsedLines);

    return grid;
}

struct CandidatePosition {
public:
    Vec2 position;
    int stepsRemaining;

    CandidatePosition(Vec2 position, int stepsRemaining) : position(position), stepsRemaining(stepsRemaining) {};

    std::size_t Hash() const {   
        return stepsRemaining + (position.x << 8) + (position.y << 16);
    }
};

void ConsiderCandidatePosition(CandidatePosition& candidate, std::queue<CandidatePosition>& candidatePositions, Grid& grid) {

    std::size_t hash = candidate.Hash();
    if (grid.visitedCandidates.contains(hash))
        return;
    grid.visitedCandidates.insert(hash);

    Cell& candidateCell = grid.GetCell(candidate.position);
    //if (candidate.stepsRemaining % 2 == 0) {
    //    // We could always step off and back until we run out of steps. Ergo this current space will be reachable in the end.
    //    if (!candidateCell.isReachable)
    //        grid.totalReachableCells++;
    //    candidateCell.isReachable = true;
    //}

    if (candidate.stepsRemaining == 0) {
        if (!candidateCell.isReachable)
            grid.totalReachableCells++;
        candidateCell.isReachable = true;
        return;
    }


    // Check four neighbours
    std::vector<Vec2> neighbourPositions{
        candidate.position + Vec2(0, -1), // North
        candidate.position + Vec2(1,0), // East
        candidate.position + Vec2(0,1), // South
        candidate.position + Vec2(-1,0) // West
    };
    for (auto& neighbourPosition : neighbourPositions) {

        // Can't move out of grid bounds
        if (!grid.IsWithinBounds(neighbourPosition))
            continue;

        // Can't move onto rocks
        auto& neighbourCell = grid.GetCell(neighbourPosition);
        if (neighbourCell.content == ROCK)
            continue;

        // Don't need to re-prove cells that are already known to be reachable
        //if (neighbourCell.isReachable)
        //    continue;

        CandidatePosition newCandidate(neighbourPosition, candidate.stepsRemaining-1);
        candidatePositions.push(newCandidate);
    }
}

void DoPart1(Grid& grid) {

    std::queue<CandidatePosition> candidatePositions;
    int startingSteps = 64;
    CandidatePosition start(grid.startingPosition, startingSteps);
    candidatePositions.push(start);

    while (candidatePositions.size() > 0) {
        CandidatePosition candidate = candidatePositions.front();
        candidatePositions.pop();
        ConsiderCandidatePosition(candidate, candidatePositions, grid);
    }

    // Count reachable cells
    long long count = 0;
    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            Vec2 pos(x, y);
            if (grid.GetCell(pos).isReachable)
                count++;
        }
    }

    std::cout << "Manual count: " << count << "\n";
    std::cout << "PART 1 ANSWER - How many spaces are reachable in " << startingSteps << " steps?: " << grid.totalReachableCells << "\n";
    // Wrong answers: 2651 (too low), 1622 (too low), 493
}

void DoPart2(Grid& grid) {
    // Not yet implemented
}

void TestHashing() {
    std::cout << "Beginning Hash Test...";

    int width = 131;
    int height = 131;
    std::unordered_set<std::size_t> visited;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int steps = 64; steps >= 0; steps--) {
                CandidatePosition c(Vec2(x, y), steps);
                auto hash = c.Hash();
                if (visited.contains(hash))
                    throw "This should not happen.";
                visited.insert(hash);
            }
        }
    }

    std::cout << "Hash Test: SUCCESS";
}

int main()
{
    TestHashing();
    std::cout << "Advent of Code - Day 21!\n";

    Grid grid = ParseInput("input.txt");

    DoPart1(grid);
    DoPart2(grid);
}