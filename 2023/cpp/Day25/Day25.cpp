#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <format>$

class Edge;

class Node {
public:
    std::string label;
    std::vector<Edge*> edges;

    Node(std::string label) : label(label) {};
};

class Edge {
public:
    Node* nodeA;
    Node* nodeB;

    Edge(Node* nodeA, Node* nodeB) : nodeA(nodeA), nodeB(nodeB) {};

    Node* GetOther(Node* start) {
        if (start == nodeA)
            return nodeB;
        if (start == nodeB)
            return nodeA;
        throw "Edge does not contain node: " + start->label;
    }

    std::string GetStringKey() {
        return nodeA->label + nodeB->label;
    }
};

class NodeGraph {
public:
    std::map<std::string, Node*> nodes;
    std::vector<Edge*> edges;

    void ListData() {
        std::cout << "---Nodes---\n";
        for (auto node : nodes) {
            std::cout << node.first << "\n";
        };

        std::cout << "---Edges---\n";
        for (auto edge : edges) {
            std::cout << edge->nodeA->label << "-" << edge->nodeB->label << "\n";
        }
    }
};

NodeGraph* ParseInput(const std::string& fileName) {
    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedLines;

    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        parsedLines.push_back(parsedLine);
    }

    NodeGraph* nodeGraph = new NodeGraph();

    // First time through to generate Nodes
    for(std::string parsedLine : parsedLines){
        auto colonIndex = parsedLine.find(':');
        parsedLine.erase(colonIndex, 1);

    
        while (parsedLine.size() > 0) {
            std::size_t nextSpaceIndex = parsedLine.find(' ');
            std::string nodeLabel = parsedLine.substr(0, nextSpaceIndex);
            if (!nodeGraph->nodes.contains(nodeLabel)) {                
                Node* newNode = new Node(nodeLabel);
                nodeGraph->nodes.insert({ nodeLabel, newNode });       
            }
            parsedLine.erase(0, nodeLabel.size() + 1);
        }       
    }

    // Second time through to generate edges
    for (auto parsedLine : parsedLines) {
        auto colonIndex = parsedLine.find(':');
        auto nodeLabel = parsedLine.substr(0, colonIndex);
        Node* firstNode = nodeGraph->nodes.find(nodeLabel)->second;
        parsedLine.erase(0, nodeLabel.size() + 2);
        while (parsedLine.size() > 0) {
            auto nextSpaceIndex = parsedLine.find(' ');
            auto edgeLabel = parsedLine.substr(0, nextSpaceIndex);            
            
            Node* secondNode = nodeGraph->nodes.find(edgeLabel)->second;

            Edge* newEdge = new Edge(firstNode, secondNode);
            nodeGraph->edges.push_back(newEdge);
            firstNode->edges.push_back(newEdge);
            secondNode->edges.push_back(newEdge);

            parsedLine.erase(0, edgeLabel.size() + 1);
        }
    }

    return nodeGraph;
}

struct WalkerState {
    int stepsSoFar = 0;
    Node* currentNode;
    Edge* previousEdge;
    WalkerState* previousState;

    WalkerState(int stepsSoFar, Node* currentNode, Edge* previousEdge, WalkerState* previousState) :stepsSoFar(stepsSoFar), currentNode(currentNode), previousEdge(previousEdge), previousState(previousState) {};

    void GetUntravelledEdges(std::vector<Edge*>& buffer) {
        buffer.clear();
        for (auto edge : currentNode->edges) {
            Node* farNode = edge->GetOther(currentNode);
            if (!HasPreviouslyVisited(farNode)) {
                buffer.push_back(edge);
            }
        }
    }

    bool HasPreviouslyVisited(Node* node) {
        WalkerState* state = this;
        while (state != nullptr) {
            if (state->currentNode == node)
                return true;

            state = state->previousState;
        }

        return false;
    }
};

struct Walker {
    int steps = 0;
    std::set<Node*> visitedNodes;
    std::set<Edge*> travelledEdges;
    Node* node;

    Walker() {}
    Walker(Node* node) : node(node) {
        visitedNodes.insert(node);
    };

    void GetUntravelledEdges(std::vector<Edge*>& buffer) {
        buffer.clear();
        for (auto edge : node->edges) {
            auto otherNode = edge->GetOther(node);
            if (!visitedNodes.contains(otherNode)) {
                buffer.push_back(edge);
            }
        }        
    }

    void Travel(Edge* edge) {
        Node* nextNode = edge->GetOther(node);
        node = nextNode;
        steps++;
        visitedNodes.insert(nextNode);
        travelledEdges.insert(edge);
    }

    Walker* Clone() {
        Walker* clone = new Walker();
        clone->steps = steps;
        clone->visitedNodes = visitedNodes;
        clone->travelledEdges = travelledEdges;
        clone->node = node;
        return clone;
    }
};

std::vector<WalkerState*> FindAllPaths(Node* start, Node* end, NodeGraph* nodeGraph) {
   
    std::queue<WalkerState*> activeWalkers;
    std::vector<WalkerState*> finishedWalkers;

    WalkerState* firstWalker = new WalkerState(0, start, nullptr, nullptr);
    activeWalkers.push(firstWalker);

    std::vector<Edge*> untravelledEdges;
    while (activeWalkers.size() > 0) {
        WalkerState* walker = activeWalkers.front();
        activeWalkers.pop();

        if (walker->currentNode == end) {
            finishedWalkers.push_back(walker);
            continue;
        }

        walker->GetUntravelledEdges(untravelledEdges);
        if (untravelledEdges.size() == 0) {
            // Dead end            
            delete walker;
            continue;
        }

        for (auto untravelledEdge : untravelledEdges) {            
            Node* farNode = untravelledEdge->GetOther(walker->currentNode);
            WalkerState* state = new WalkerState(walker->stepsSoFar + 1, farNode, untravelledEdge, walker);            
            activeWalkers.push(state);
        }               
    }
    return finishedWalkers;
}

int CountNodesConnectedTo(Node* node) {
    std::unordered_set<Node*> visitedNodes;
    std::queue<Node*> frontier;
    frontier.push(node);
    while (frontier.size() > 0) {
        Node* focusNode = frontier.front();
        frontier.pop();
        visitedNodes.insert(focusNode);
        for (auto neighbour : focusNode->edges)
        {
            Node* otherNode = neighbour->GetOther(focusNode);
            if (visitedNodes.contains(otherNode))
                continue;
            frontier.push(otherNode);
        }
    }

    return visitedNodes.size();
}

Edge* FindMostTravelledEdge(NodeGraph* nodeGraph) {
    std::vector<Node*> nodes;
    for (auto [key, value] : nodeGraph->nodes) {
        nodes.push_back(value);
    }

    std::unordered_map<Edge*, int> edgeScores;
    for (int i = 0; i < nodes.size() - 1; i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            Node* start = nodes[i];
            Node* end = nodes[j];

            auto allWalkerStates = FindAllPaths(start, end, nodeGraph);
            for (const auto walkerState : allWalkerStates) {
                auto state = walkerState;
                while (state != nullptr) {
                    if(state->previousEdge != nullptr)
                        edgeScores[state->previousEdge] += 1;
                    state = state->previousState;
                }                
            }
        }
    }

    Edge* mostTravelledEdge = nodeGraph->edges.front();  
    for (auto& kvp : edgeScores) {
        if (kvp.second > edgeScores[mostTravelledEdge]) {
            mostTravelledEdge = kvp.first;
        }
    }
       
    return mostTravelledEdge;
}

void DeleteEdgeFromNodeGraph(Edge* edge, NodeGraph* nodeGraph) {
    std::erase(nodeGraph->edges, edge);
    std::erase(edge->nodeA->edges, edge);
    std::erase(edge->nodeB->edges, edge);
}

auto GlobalMinCut(std::vector<std::vector<int>>& mat) {
    std::pair<int, std::vector<int>> best = { std::numeric_limits<int>::max(), {} };
    int n = mat.size();
    std::vector<std::vector<int>> co(n);

    for (int i = 0; i < n; i++)
        co[i] = { i };

    for (int ph = 1; ph < n; ph++) {
        std::vector<int> w = mat[0];
        size_t s = 0, t = 0;
        for (int it = 0; it < n - ph; it++) { // O(V^2) -> O(E log V) with prio. queue
            w[t] = std::numeric_limits<int>::min();
            s = t, t = std::max_element(w.begin(), w.end()) - w.begin();
            for (int i = 0; i < n; i++) w[i] += mat[t][i];
        }
        best = min(best, { w[t] - mat[t][t], co[t] });
        co[s].insert(co[s].end(), co[t].begin(), co[t].end());
        for (int i = 0; i < n; i++) mat[s][i] += mat[t][i];
        for (int i = 0; i < n; i++) mat[i][s] = mat[s][i];
        mat[0][t] = std::numeric_limits<int>::min();
    }
    return best;    
}

void DoPart1(NodeGraph* nodeGraph) {    
    auto edge1 = FindMostTravelledEdge(nodeGraph);
    DeleteEdgeFromNodeGraph(edge1, nodeGraph);    
 
    auto edge2 = FindMostTravelledEdge(nodeGraph);
    DeleteEdgeFromNodeGraph(edge2, nodeGraph);

    auto edge3 = FindMostTravelledEdge(nodeGraph);
    DeleteEdgeFromNodeGraph(edge3, nodeGraph);

    auto firstNode = (*nodeGraph->nodes.begin()).second;
    int netSizeA = CountNodesConnectedTo(firstNode);
    int netSizeB = nodeGraph->nodes.size() - netSizeA;

    auto productOfTwoNetSizes = netSizeA * netSizeB;
    std::cout << "PART 1 ANSWER - Product of two disjoint network sizes: " << productOfTwoNetSizes << "\n";
}

void DoPart2(NodeGraph* nodeGraph) {
    // Not implemented yet
}

int main()
{
    std::cout << "Advent of Code - Day 25!\n";

    std::string fileName = "input.txt";   
    if (false) {
        fileName = "test_input.txt";    
    }

    std::ifstream in(fileName);
    std::string line;
    std::vector<std::pair<int, int>> connections;
    std::map<std::string, int> nameMap;

    auto GetIndex = [&nameMap](const std::string& s) {
        if (nameMap.find(s) != nameMap.cend())
            return nameMap[s];
        int index = nameMap.size();
        nameMap[s] = index;
        return index;
        };

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string name;
        ss >> name;
        name = name.substr(0, name.size() - 1);
        int baseIndex = GetIndex(name);
        while (ss >> name)
            connections.emplace_back(baseIndex, GetIndex(name));
    }

    std::vector<std::vector<int>> adj(nameMap.size(), std::vector<int>(nameMap.size(), 0));
    for (const auto& p : connections)
        adj[p.first][p.second] = adj[p.second][p.first] = 1;

    auto result = GlobalMinCut(adj);
    std::cout << std::format("Part 1: {} \n", result.second.size() * (nameMap.size() - result.second.size()));

    //auto nodeGraph = ParseInput(fileName);
    //nodeGraph->ListData();

    //DoPart1(nodeGraph);
    //DoPart2(nodeGraph);
}