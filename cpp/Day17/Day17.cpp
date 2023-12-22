#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <map>
#include <queue>
#include <limits>

class Vec2 {
public:
    int x{};
    int y{};
    Vec2();
    Vec2(int x, int y) : x(x), y(y) {}

    //bool operator<(const Vec2& other) const { return x < other.x && y < other.y; };
};

static Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

static Vec2 operator-(Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

static bool operator==(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

static bool operator!=(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

const Vec2 NORTH = Vec2(0, -1);
const Vec2 SOUTH = Vec2(0, 1);
const Vec2 EAST = Vec2(1, 0);
const Vec2 WEST = Vec2(-1, 0);

struct Node {
public:
    Vec2 position{};
    //Vec2 previousPosition;
    Node* previousNode;
    int steps = 0;
    long costSoFar{};
   
    Node(Vec2 position, Node* previousNode, int steps, long costSoFar) : position(position), previousNode(previousNode), steps(steps), costSoFar(costSoFar) {}

    bool operator==(const Node& other) const { return position == other.position && &previousNode == &other.previousNode && steps == other.steps && costSoFar == other.costSoFar; }
    bool operator<(const Node& other) const { return costSoFar < other.costSoFar; }    

    long long Hash() {
        long hash = (position.x) + (position.y << 4);
        //if (previousNode)
          //  hash += (previousNode->position.x << 12) + (previousNode->position.y << 16);
       
        return hash;
    }
};


template <typename T>
class Grid {
public:
    std::vector<std::vector<int>> content;
    int width;
    int height;

    Grid(const std::vector<std::string>& input) {
        width = input[0].size();
        height = input.size();
        for (int y = 0; y < height; y++) {
            std::vector<int> row;
            for (int x = 0; x < width; x++) {
                int value = input[y][x] - '0';
                row.push_back(value);
            }
            content.push_back(row);
        }
    } 

    T Get(Vec2 position) const {
        return content[position.y][position.x];
    }

    bool IsWithinBounds(const Vec2& position) const {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

    void PrintGrid() const {
        std::cout << "---Grid---\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto value = Get(Vec2(x, y));
                std::cout << std::to_string(value);
            }
            std::cout << "\n";
        }
        std::cout << "----------\n";
    }
};

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

std::vector<Node*> FindEndNodes(const Vec2 & startPosition, const Vec2 & endPosition, const Grid<int>&grid) {
    
    auto nodeValueComparison = [](const Node* lhs, const Node* rhs) {
        return lhs->costSoFar > rhs->costSoFar;
        };    
    auto frontier = std::priority_queue<Node*, std::deque<Node*>, decltype(nodeValueComparison)>();
    Node* startNode = new Node(Vec2(0, 0), nullptr, 0, 0);
    frontier.push(startNode);
       
    long cumulativeHeatLoss = LONG_MAX;    
    std::vector<Node*> endNodes{};

    std::set<long long> visited{};   
    while (!frontier.empty()) {
        Node* frontierNode = frontier.top();
        frontier.pop();
        visited.insert(frontierNode->Hash());

        if (frontierNode->position == endPosition) {
            endNodes.push_back(frontierNode);
            continue;
        }

        auto neighbourPositions = GetValidNeighboursPositions(frontierNode, grid);
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

            // Can't walk more than three steps in the same direction
            if (frontierNode->steps > 3)
                continue;

            long long neighbourNodeHash = neighbourNode->Hash();            
            if (!visited.contains(neighbourNodeHash))
                frontier.push(neighbourNode);                              
        }
    }
    
    return endNodes;
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

    long cumulativeHeatLoss = LONG_MAX;
    Node* startNode = new Node(Vec2(0, 0), nullptr, 0, 0);
    auto endNodes = FindEndNodes(Vec2(0, 0), Vec2(grid.width - 1, grid.height - 1), grid);
    for (auto endNode : endNodes) {
        auto path = GeneratePathFromEndNode(startNode, endNode);
        PrintPathOnGrid(path, grid);
        if (endNode->costSoFar < cumulativeHeatLoss)
            cumulativeHeatLoss = endNode->costSoFar;
    }

    std::cout << "ANSWER PART 1 - Cumulative heat loss: " << cumulativeHeatLoss << "\n";
    //std::cout << "ANSWER PART 1 - Cumulative heat loss: " << path << "\n";

    return 0;
}