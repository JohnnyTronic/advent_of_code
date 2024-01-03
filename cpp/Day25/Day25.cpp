#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include <unordered_set>
#include <queue>

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

        return nodeA;
    }
};

class NodeGraph {
public:
    std::map<std::string, Node*> nodes;
    std::vector<Edge*> edges;
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

int BreadthFirstNodeCount(NodeGraph* nodeGraph, std::unordered_set<Edge*> edgeBlacklist) {
    std::unordered_set<Node*> visited;
    std::queue<Node*> frontier;
    auto startNode = nodeGraph->nodes.begin()->second;
    frontier.push(startNode);
    while (frontier.size() > 0) {
        Node* node = frontier.front();
        frontier.pop();
        if (visited.contains(node))
            continue;
        visited.insert(node);
        for (auto edge : node->edges) {
            if (edgeBlacklist.contains(edge))
                continue;
            frontier.push(edge->GetOther(node));
        }
    }
    return visited.size();
}

void DoPart1(NodeGraph* nodeGraph) {

    int edgeCount = nodeGraph->edges.size();
    std::unordered_set<Edge*> edgeBlacklist;
    for (int i = 0; i < edgeCount - 2; i++) {
        std::cout << "i: " << i << " of " << edgeCount << "\n";
        for (int j = 1; j < edgeCount - 1; j++) {
            for (int k = 2; k < edgeCount; k++) {
                edgeBlacklist.clear();
                Edge* edgeA = nodeGraph->edges.at(i);
                Edge* edgeB = nodeGraph->edges.at(j);
                Edge* edgeC = nodeGraph->edges.at(k);
                edgeBlacklist.insert({ edgeA, edgeB, edgeC});

                int nodeCount = BreadthFirstNodeCount(nodeGraph, edgeBlacklist);
                if (nodeCount < nodeGraph->nodes.size()) {
                    int otherNodeCount = nodeGraph->nodes.size() - nodeCount;
                    double product = nodeCount * otherNodeCount;
                    std::cout << "PART 1 ANSWER - Product of size fo the two disjoint graphs: " << product << ", Blacklisted Edges: ";
                    for (auto edge : edgeBlacklist) {
                        std::cout << edge->nodeA->label << ":" << edge->nodeB->label << ", ";
                    }
                    std::cout << "\n";
                    return;
                }
            }
        }
    }
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

    auto nodeGraph = ParseInput(fileName);

    DoPart1(nodeGraph);
    DoPart2(nodeGraph);
}