#include "Vec2.h"
#include "Node.h"
#include "Grid.h"
#include "NodeCostCompare.h"
#include "Tests.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <unordered_set>
#include <map>
#include <queue>
#include <limits>


Node* FindEndNode(const Vec2 & startPosition, const Vec2 & endPosition, int minSteps, int maxSteps, const Grid<std::size_t>&grid) {          
    
    auto frontier = std::priority_queue<Node*, std::vector<Node*>, NodeCostCompare>();
    Node* startNode = new Node(Vec2(0, 0), nullptr, 0, 0);
    frontier.push(startNode);
   
    std::unordered_set<std::size_t> visited{};   
    //std::set<std::size_t> visited{};   
    while (!frontier.empty()) {
        Node* frontierNode = frontier.top();
        frontier.pop();

        auto frontierNodeHash = frontierNode->Hash();
        if (visited.contains(frontierNodeHash))
            continue;
        visited.insert(frontierNodeHash);

        if (frontierNode->position == endPosition) {   
            Node* frontierTop = frontier.top();
            long topCost = frontierTop->costSoFar;
            while (!frontier.empty()) {
                Node* nextFrontierTop = frontier.top();
                frontier.pop();
                assert(topCost <= nextFrontierTop->costSoFar && "Priority Queue not working...?");
                   
                frontierTop = nextFrontierTop;
            }
            return frontierNode;
            //continue;
        }               

        auto neighbourPositions = GetValidNeighboursPositions(frontierNode, minSteps, maxSteps, grid);
        for (auto neighbourPosition : neighbourPositions) {
            int steps = frontierNode->steps + 1;

            if (frontierNode->previousNode) {                
                Vec2 previousStepDirection = frontierNode->position - frontierNode->previousNode->position;
                Vec2 currentStepDirection = neighbourPosition - frontierNode->position;
                if (currentStepDirection != previousStepDirection)
                    steps = 1;
            }            
                        
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

void PrintPathOnGrid(const std::deque<Node*>& path, const Grid<std::size_t>& grid) {

    std::cout << "---PathGrid---\n";
    for (std::size_t y = 0; y < grid.height; y++) {
        for (std::size_t x = 0; x < grid.width; x++) {
            Vec2 currentPos = Vec2(x, y);
            auto value = grid.Get(currentPos);
            bool onPath = false;
            for (std::size_t n = 0; n < path.size() - 1; n++) {
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
    if (false) {
        std::cout << "Size of int: " << sizeof(int) << "\n";
        TestPriorityQueue();
        TestNodeHashing();
        TestGetPossibleNextDirections();
        TestGetValidNeighbourPositions();   
    }
     
    std::cout << "Advent of Code - Day 17!\n";

    std::ifstream ifs("input.txt");
    std::string parsedLine;
    std::vector<std::string> parsedLines;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    Grid<std::size_t> grid(parsedLines);
    grid.PrintGrid();
        
    Node* startNode = new Node(Vec2(0, 0), nullptr, 0, 0);
    Node* endNode = FindEndNode(Vec2(0, 0), Vec2(grid.width - 1, grid.height - 1), 1, 3, grid);
    auto path = GeneratePathFromEndNode(startNode, endNode);
    PrintPathOnGrid(path, grid);
    
    std::cout << "ANSWER PART 1 - Cumulative heat loss: " << endNode->costSoFar << "\n";
    //std::cout << "ANSWER PART 1 - Cumulative heat loss: " << path << "\n";
    // Wrongs answers: 821, 829, 830, 921, 1034
    // 
    // LEARNING - Part A solution is 817 based on code from: https://github.com/coolguy1842/adventofcode/blob/master/2023/src/include/days/Day17/Day17.hpp
    // Now to figure out what's wrong with MY code...

    // Part 2 - Ultra crucible
    {
        Node* endNode = FindEndNode(Vec2(0, 0), Vec2(grid.width - 1, grid.height - 1), 4, 10, grid);
        std::cout << "ANSWER PART 2 - Cumulative heat lost for ultra crucible: " << endNode->costSoFar << "\n";
    }

    // Part 2 - Candidate answer 925
    return 0;
}