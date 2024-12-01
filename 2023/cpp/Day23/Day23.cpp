#include "Vec2.h"
#include "Grid.h"
#include "GridWalker.h"
#include "NodeGraph.h"
#include "GraphWalker.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

Grid ParseInput(const std::string& fileName) {
  
    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedLines;
   
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);      
    }

    Grid grid(parsedLines);
    return grid;
}

struct Candidate {
public:
    Vec2 position;
    int distance;

    std::size_t operator()(const Candidate& c) {
        return static_cast<std::size_t>(c.position.x) + (static_cast<std::size_t>(c.position.y) << 16) + (static_cast<std::size_t>(distance) << 32);
    }   
};

std::vector<Vec2> NeighbourOffsets{ Vec2(0,-1), Vec2(1,0), Vec2(0,1), Vec2(-1,0) };
Vec2 neighbourPosition;
Cell* currentCell;
Cell* neighbourCell;
void GetValidNeighbours(std::vector<Vec2>& buffer, const Vec2& position, const Grid& grid, const std::unordered_set<Vec2, Vec2::HashFunction>& visited, const bool& slippery = true) {

    currentCell = grid.GetAt(position);
    buffer.clear();

    for (const auto& offset : NeighbourOffsets) {        
        neighbourPosition = position + offset;
        
        // Can't walk outside of map
        if(!grid.IsWithinBounds(neighbourPosition))
            continue;

        // Can't backtrack
        if (visited.contains(neighbourPosition))
            continue;

        neighbourCell = grid.GetAt(neighbourPosition);

        // Can't walk into forest
        if (neighbourCell->terrain == FOREST)
            continue;

        if (slippery) {
            if (currentCell->terrain == NORTHSLOPE && offset != NORTH)
                continue;
            if (currentCell->terrain == EASTSLOPE && offset != EAST)
                continue;
            if (currentCell->terrain == SOUTHSLOPE && offset != SOUTH)
                continue;
            if (currentCell->terrain == WESTSLOPE && offset != WEST)
                continue;
        }

        buffer.push_back(neighbourPosition);
    }
}



void DoPart1(const std::string& fileName) {
    Grid grid = ParseInput(fileName);

    Vec2 startPosition(1, 0);
    Vec2 endPosition(grid.width - 2, grid.height - 1);

    //grid.PrintGrid();

    // Find longest path from start to end
    struct CustomComparison {
        bool operator()(const Candidate lhs, const Candidate rhs) const { return lhs.distance < rhs.distance; }
    };
    std::priority_queue<Candidate, std::vector<Candidate>, CustomComparison> frontier;
    std::unordered_set<Vec2, Vec2::HashFunction> visited;

    std::vector<GridWalker*> allWalkers{ new GridWalker(startPosition, 0, visited) };
    std::queue<GridWalker*> activeWalkers;
    for (auto walker : allWalkers)
        activeWalkers.push(walker);
     
    std::vector<Vec2> validNeighbours;
    while (activeWalkers.size() > 0) {
        GridWalker* focusGridWalker = activeWalkers.front();

        if (focusGridWalker->position == endPosition) {
            activeWalkers.pop();
            std::cout << "Active walkers: " << std::to_string(activeWalkers.size()) << ", END POSITION REACHED\n";
            continue;
        }

        GetValidNeighbours(validNeighbours, focusGridWalker->position, grid, focusGridWalker->visited);

        if (validNeighbours.size() == 0) {
            activeWalkers.pop();
            
            std::erase_if(allWalkers, [&focusGridWalker](GridWalker* walker) {return walker == focusGridWalker; });
            std::cout << "Active walkers: " << std::to_string(activeWalkers.size()) << ", DEAD END\n";
            continue;
        }
        else if (validNeighbours.size() == 1) {
            focusGridWalker->MoveTo(validNeighbours[0]);
            continue;
        }
        else {
            for (int i = 1; i < validNeighbours.size(); i++) {
                GridWalker* newGridWalker = focusGridWalker->Clone();
                newGridWalker->MoveTo(validNeighbours[i]);
                activeWalkers.push(newGridWalker);
                allWalkers.push_back(newGridWalker);
                std::cout << "Active walkers: " << std::to_string(activeWalkers.size()) << ", FORK\n";
            }
            focusGridWalker->MoveTo(validNeighbours[0]);
        }
    }

    int longestPath = allWalkers[0]->steps;
    GridWalker* longestGridWalker = allWalkers[0];
    for (int i = 1; i < allWalkers.size(); i++) {
        GridWalker* testGridWalker = allWalkers[i];
        if (testGridWalker->steps > longestPath) {
            longestPath = testGridWalker->steps;
            longestGridWalker = testGridWalker;
        }
    }

    std::cout << "PART 1 ANSWER - Longest walking path: " << longestGridWalker->steps << "\n";
}

void DoPart2(const std::string& fileName) {
    Grid grid = ParseInput(fileName);
    NodeGraph nodeGraph(grid);

    Vec2 startPosition(1, 0);
    Vec2 endPosition(grid.width - 2, grid.height - 1);
    Node* startNode = nodeGraph.nodes[startPosition];
    Node* endNode = nodeGraph.nodes[endPosition];
        
    std::unordered_set<Node*, NodeHashFunction> visitedNodes{ startNode };
    GraphWalker* firstGraphWalker = new GraphWalker(startNode, visitedNodes, 0);
    //std::vector<GraphWalker*> finishedGraphWalkers{};
    std::queue<GraphWalker*> activeGraphWalkers;
    activeGraphWalkers.push(firstGraphWalker);
    
    long long longestWalk = 0;
    while (activeGraphWalkers.size() > 0) {
        GraphWalker* graphWalker = activeGraphWalkers.front();

        if (graphWalker->currentNode == endNode) {
            //finishedGraphWalkers.push_back(graphWalker);
            if (graphWalker->totalDistance > longestWalk) {
                longestWalk = graphWalker->totalDistance;
                std::cout << "New longest walk: " << std::to_string(longestWalk) << "\n";
            }
            //std::cout << "Active graph walkers: " << std::to_string(activeGraphWalkers.size()) << ", END POSITION REACHED - " << graphWalker->totalDistance << "\n";
            activeGraphWalkers.pop();
            delete graphWalker;
            continue;
        }

        auto untravelledEdges = graphWalker->GetUntravelledEdges();
        if (untravelledEdges.size() == 0) {
            // This GraphWalker has no where new to go
            activeGraphWalkers.pop();
            continue;
        }

        for (int i = 1; i < untravelledEdges.size(); i++) {
            GraphWalker* newGraphWalker = graphWalker->Clone();
            Edge* untravelledEdge = untravelledEdges[i];
            newGraphWalker->TravelAlong(untravelledEdge);
            activeGraphWalkers.push(newGraphWalker);                
            //std::cout << "Active walkers: " << std::to_string(activeWalkers.size()) << ", FORK\n";
        }
        graphWalker->TravelAlong(untravelledEdges[0]);
    }

    /*GraphWalker* longestGraphWalker = finishedGraphWalkers[0];
    for (int i = 1; i < finishedGraphWalkers.size(); i++) {
        GraphWalker* testGraphWalker = finishedGraphWalkers[i];
        if (testGraphWalker->totalDistance > longestGraphWalker->totalDistance) {            
            longestGraphWalker = testGraphWalker;
        }
    }*/

    //std::cout << "PART 2 ANSWER - Longest walking path without slippery slopes: " << longestGraphWalker->totalDistance << "\n";
    std::cout << "PART 2 ANSWER - Longest walking path without slippery slopes: " << longestWalk << "\n";
}

void TestHashing(const std::string& inputFile) {
    Grid grid = ParseInput(inputFile);
    std::unordered_set<Vec2, Vec2::HashFunction> visited;
    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            Vec2 pos(x, y);            
            if (visited.contains(pos))
                throw "Something's wrong with the Vec2 hash function.";
            visited.insert(pos);
        }
    }
}

int main()
{
    std::cout << "Advent of Code - Day 23!\n";

    std::string inputFile = "input.txt";
    TestHashing(inputFile);
    //DoPart1(inputFile);
    DoPart2(inputFile);
}