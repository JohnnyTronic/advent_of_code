#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <map>
#include <queue>

class Vec2 {
public:
    int x;
    int y;
    Vec2(int x, int y) : x(x), y(y) {}
};

static Vec2 operator+(Vec2 lhs, Vec2 rhs) {
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

class Node {
public:
    int value;
    Vec2 position;
    long priority{};
    Node(int value, Vec2 position) : value(value), position(position) {}
};

class Grid {
public:
    std::vector<std::vector<Node*>> content;
    int width;
    int height;

    Grid(const std::vector<std::string>& input) {
        width = input[0].size();
        height = input.size();
        for (int y = 0; y < height; y++) {
            std::vector<Node*> row;
            for (int x = 0; x < width; x++) {
                int value = input[y][x] - '0';
                Node* node = new Node(value, Vec2(x,y));
                row.push_back(node);
            }
            content.push_back(row);
        }
    } 

    Node* GetNodeAt(Vec2 position) {
        return content[position.y][position.x];
    }

    std::vector<Node*> GetNodeNeighbours(Node* node) {
        const Vec2& position = node->position;
        std::vector<Vec2> neighbourPositions;
        neighbourPositions.push_back(position + Vec2(0, -1));
        neighbourPositions.push_back(position + Vec2(0, 1));
        neighbourPositions.push_back(position + Vec2(-1, 0));
        neighbourPositions.push_back(position + Vec2(1, 0));

        std::vector<Node*> neighbours;
        for (auto pos : neighbourPositions) {
            if (IsWithinBounds(pos))
                neighbours.push_back(GetNodeAt(pos));
        }

        return neighbours;
    }

    bool IsWithinBounds(const Vec2& position) {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

    long Cost(Node* from, Node* to) {
        return to->value;
    }

    void PrintGrid() {
        std::cout << "---Grid---\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto node = GetNodeAt(Vec2(x, y));
                std::cout << std::to_string(node->value);
            }
            std::cout << "\n";
        }
        std::cout << "----------";
    }
};

std::deque<Node*> FindPath(Vec2 start, Vec2 end, Grid& grid) {
    auto nodeValueComparison = [](const Node* lhs, const Node* rhs) {
        return lhs->priority > rhs->priority;
        };
    auto frontier = std::priority_queue<Node*,std::vector<Node*>,decltype(nodeValueComparison)>(nodeValueComparison);
    auto startNode = grid.GetNodeAt(start);
    frontier.push(startNode);

    auto cameFromMap = std::map<Node*, Node*>();
    std::map<Node*, long> costSoFarMap{};
    costSoFarMap[startNode] = 0;
    int consecutiveCount = 0;
    Vec2 currentDirection = Vec2(0, 0); 
    while (!frontier.empty()) {
        Node* current = frontier.top();
        frontier.pop();

        auto neighbours = grid.GetNodeNeighbours(current);      
        for (auto neighbour : neighbours) {
            int newCost = costSoFarMap[current] + grid.Cost(current, neighbour);
            costSoFarMap[neighbour] = newCost;
            if (!cameFromMap.contains(neighbour)) {
                neighbour->priority = newCost;
                frontier.push(neighbour);
                cameFromMap[neighbour] = current;
            }
        }
    }

    // Walk backwards from end to start via cameFromMap
    std::deque<Node*> path;
    auto endNode = grid.GetNodeAt(end);
    auto previous = endNode;
    path.push_front(previous);
    while (previous->position != startNode->position && cameFromMap.contains(previous)) {
        previous = cameFromMap[previous];
        path.push_front(previous);
    }

    return path;
};

char GetArrowFor(Vec2& direction) {
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

void PrintPathOnGrid(std::deque<Node*> path, Grid& grid) {

    std::cout << "---PathGrid---\n";
    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            Vec2 currentPos = Vec2(x, y);
            auto node = grid.GetNodeAt(currentPos);
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
                std::cout << std::to_string(node->value);
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

    Grid grid(parsedLines);
    grid.PrintGrid();

    auto path = FindPath(Vec2(0, 0), Vec2(grid.width - 1, grid.height - 1), grid);

    PrintPathOnGrid(path, grid);

    return 0;
}
