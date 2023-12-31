#include "Vec2.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

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

    CandidatePosition(Vec2 position = Vec2(0,0), int stepsRemaining = 0) : position(position), stepsRemaining(stepsRemaining) {};

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
    // Referencing the online AoC 2023 subreddit, it seems Part 2 is solved thanks to some particularities in the input data
    // I.e. the garden pattern has empty edges, the starting row/column is clear, and there are diamond-shaped "highways" of clean cells
    // Further, the target number of steps in multiple of the original grid dimensions
    // As such, there are some geometric properties that can be exploited to calculate the finally tally of reachable positions without actually needing to "walk" them via code
    // Due to the above, the rock placements don't actually prevent the elf from reaching adjacent garden plots
    // See explanations such as:
    // https://work.njae.me.uk/2023/12/29/advent-of-code-2023-day-21/
    // https://github.com/villuna/aoc23/wiki/A-Geometric-solution-to-advent-of-code-2023,-day-21

    std::unordered_map<std::size_t, CandidatePosition> visited;
    std::queue<CandidatePosition> frontier;
    CandidatePosition startPosition(grid.startingPosition, 0);
    frontier.push(startPosition);
    while (frontier.size() > 0) {
        auto candidate = frontier.front();
        frontier.pop();
        if(visited.contains(candidate.position.Hash()))
            continue;
        visited[candidate.position.Hash()] = candidate;

       std::vector<Vec2> neighbourPositions{
       candidate.position + Vec2(0, -1), // North
       candidate.position + Vec2(1,0), // East
       candidate.position + Vec2(0,1), // South
       candidate.position + Vec2(-1,0) // West
        };

       for (auto& neighbourPosition : neighbourPositions) {
           if (!grid.IsWithinBounds(neighbourPosition))
               continue;
           auto& neighbourCell = grid.GetCell(neighbourPosition);
           if (neighbourCell.content == ROCK)
               continue;
           if (visited.contains(neighbourPosition.Hash()))
               continue;

           CandidatePosition neighbourCandidate(neighbourPosition, candidate.stepsRemaining + 1);
           frontier.push(neighbourCandidate);           
       }
    }

    // At this point, we should all the cells in the grid mapped out with how many steps it took for the elf to reach them

    // Count how many have even steps less than 64
    int evenFullCount = 0;
    int oddFullCount = 0;
    int evenCornerCount = 0;
    int oddCornerCount = 0;
    for (auto& candidate : visited) {
        int steps = candidate.second.stepsRemaining;
        if (steps % 2 == 0)
            evenFullCount++;

        if (steps % 2 == 1)
            oddFullCount++;

        if (steps % 2 == 0 && steps > 65)
            evenCornerCount++;

        if (steps % 2 == 1 && steps > 65)
            oddCornerCount++;
    }

    long long targetSteps = 26501365;
    long long n = (targetSteps - (grid.width / 2)) / grid.width;
    if (n != 202300)
        throw "Incorrect dimensions";
    long long evenTiles = n * n;
    long long oddTiles = (n + 1) * (n + 1);

    long long answer2 = oddTiles * oddFullCount + evenTiles * evenFullCount - ((n + 1) * oddCornerCount) + (n * evenCornerCount);

    std::cout << "PART 2 ANSWER - Total reachable positions in infinitely tiling garden: " << answer2 << "\n";

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
    //TestHashing();
    std::cout << "Advent of Code - Day 21!\n";

    Grid grid = ParseInput("input.txt");

    DoPart1(grid);
    DoPart2(grid);
}