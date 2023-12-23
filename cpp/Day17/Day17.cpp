#include "Vec2.h"
#include "Node.h"
#include "Grid.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <map>
#include <queue>
#include <limits>



std::vector<Vec2> GetPossibleNextDirections(const Vec2& currentDirection) {
    if (currentDirection == NORTH)
        return { WEST, NORTH, EAST };

    if (currentDirection == EAST)
        return { NORTH, EAST, SOUTH };

    if (currentDirection == SOUTH)
        return { EAST, SOUTH, WEST };

    if (currentDirection == WEST)
        return { SOUTH,WEST,NORTH };

    return { NORTH, EAST, SOUTH, WEST };
}

std::vector<Vec2> GetValidNeighboursPositions(const Node* node, const Grid<int>& grid) {

    // Since the crucible can't turn around, we eliminate "backwards" direction
    Vec2 currentDirection = Vec2(0, 0);
    if (node->previousNode)
        currentDirection = node->position - node->previousNode->position;
    
    std::vector<Vec2> possibleDirections = GetPossibleNextDirections(currentDirection);  
    std::vector<Vec2> nonBackwardNeighbours;
    for (auto possibleDirection : possibleDirections)
        nonBackwardNeighbours.push_back(possibleDirection + node->position);
    
    // Eliminate positions that are outside grid bounds    
    std::vector<Vec2> inBoundsNeighbours;
    std::copy_if(nonBackwardNeighbours.begin(), nonBackwardNeighbours.end(), std::back_inserter(inBoundsNeighbours), [&grid](Vec2 position) {
        return grid.IsWithinBounds(position);
        });
    
    return inBoundsNeighbours;
}

struct NodeCostCompare {
    bool operator()(const Node* a, const Node* b) {
        return a->costSoFar < b->costSoFar;
    }
};

Node* FindEndNode(const Vec2 & startPosition, const Vec2 & endPosition, const Grid<int>&grid) {
    
    auto nodeValueComparison = [](const Node* lhs, const Node* rhs) -> bool {
        return lhs->costSoFar > rhs->costSoFar;
        };    
    //auto frontier = std::priority_queue<Node*, std::deque<Node*>, decltype(nodeValueComparison)>();
    auto frontier = std::priority_queue<Node*, std::vector<Node*>, NodeCostCompare>();
    Node* startNode = new Node(Vec2(0, 0), nullptr, 0, 0);
    frontier.push(startNode);
   
    std::set<std::string> visited{};   
    while (!frontier.empty()) {
        Node* frontierNode = frontier.top();
        frontier.pop();

        auto frontierNodeHash = frontierNode->Hash();
        if (visited.contains(frontierNodeHash))
            continue;

        if (frontierNode->position == endPosition) {
            //endNodes.push_back(frontierNode);

            Node* popper = frontier.top();
            long latestPop = popper->costSoFar;
            while (!frontier.empty()) {
                Node* nextPopper = frontier.top();
                frontier.pop();
                if (nextPopper->costSoFar > popper->costSoFar)
                    throw "Priority queue not working...?";
                popper = nextPopper;
            }
            return frontierNode;
            //continue;
        }
                
        visited.insert(frontierNodeHash);

        auto neighbourPositions = GetValidNeighboursPositions(frontierNode, grid);
        for (auto neighbourPosition : neighbourPositions) {
            int steps = frontierNode->steps + 1;

            if (frontierNode->previousNode) {                
                Vec2 previousStepDirection = frontierNode->position - frontierNode->previousNode->position;
                Vec2 currentStepDirection = neighbourPosition - frontierNode->position;
                if (currentStepDirection != previousStepDirection)
                    steps = 1;
            }            

            // Can't walk more than three steps in the same direction
            if (steps > 3)
                continue;
            
            // Candidate neighbourNode
            Node* neighbourNode = new Node(neighbourPosition, frontierNode, steps, frontierNode->costSoFar + grid.Get(neighbourPosition));
            frontier.push(neighbourNode); 
        }
    }
    
    throw "Should have found EndNode before this.";
};

char GetArrowFor(const Vec2& direction) {
    if (direction == Vec2(1, 0))
        return '>';

    if (direction == Vec2(-1, 0))
        return '<';

    if (direction == Vec2(0, -1))
        return '^';

    if (direction == Vec2(0, 1))
        return 'v';

    throw "Unexpected delta ? ";
}

std::deque<Node*> GeneratePathFromEndNode(const Node* startNode, Node* endNode) {
    // Walk backwards from end to start via cameFromMap
    std::deque<Node*> path;
    Node* previous = endNode;
    while (previous && previous->position != startNode->position) {
        path.push_front(previous);
        if (previous->previousNode)
            previous = previous->previousNode;
    }

    return path;
}

void PrintPathOnGrid(const std::deque<Node*>& path, const Grid<int>& grid) {

    std::cout << "---PathGrid---\n";
    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            Vec2 currentPos = Vec2(x, y);
            auto value = grid.Get(currentPos);
            bool onPath = false;
            for (int n = 0; n < path.size() - 1; n++) {
                auto pathNode = path[n];
                if (pathNode->position == currentPos) {
                    onPath = true;
                    auto nextPathNode = path[n + 1];
                    auto delta = nextPathNode->position - pathNode->position;
                    char arrow = GetArrowFor(delta);
                    std::cout << arrow;
                    break;
                }
            }

            if (!onPath)
                std::cout << std::to_string(value);
        }
        std::cout << "\n";
    }
    std::cout << "--------------\n";

    return;
}

int main()
{   
    std::cout << "Advent of Code - Day 16!\n";

    std::ifstream ifs("test_input.txt");
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    Grid<int> grid(parsedLines);
    grid.PrintGrid();
        
    Node* startNode = new Node(Vec2(0, 0), nullptr, 0, 0);
    Node* endNode = FindEndNode(Vec2(0, 0), Vec2(grid.width - 1, grid.height - 1), grid);
    /*for (auto endNode : endNodes) {
        auto path = GeneratePathFromEndNode(startNode, endNode);
        PrintPathOnGrid(path, grid);
        if (endNode->costSoFar < cumulativeHeatLoss)
            cumulativeHeatLoss = endNode->costSoFar;
    }*/

    std::cout << "ANSWER PART 1 - Cumulative heat loss: " << endNode->costSoFar << "\n";
    //std::cout << "ANSWER PART 1 - Cumulative heat loss: " << path << "\n";
    // Wrongs answers: 829, 830, 921
    return 0;
}