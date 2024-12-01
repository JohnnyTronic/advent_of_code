#pragma once
#include "Node.h"
#include "Grid.h"
#include "GraphMapper.h"
#include "NodeHashFunction.h"
#include <unordered_map>
#include <queue>

struct NodeGraph {
public:
    std::unordered_map<Vec2, Node*, Vec2::HashFunction> nodes{};
    std::unordered_map<Node*, Edge*, NodeHashFunction> edges{};

    NodeGraph(const Grid& grid) {
        Vec2 startPosition(1, 0);
        Vec2 endPosition(grid.width - 2, grid.height - 1);

        Node* startNode = new Node(startPosition);
        nodes.insert({ startNode->position, startNode });

        Node* endNode = new Node(endPosition);
        nodes.insert({ endNode->position, endNode });

        GraphMapper* firstGraphMapper = new GraphMapper(startPosition, startPosition, startNode);
        std::queue<GraphMapper*> graphMappers;
        graphMappers.push(firstGraphMapper);

        while (graphMappers.size() > 0) {
            auto& graphMapper = graphMappers.front();
            auto nextPositions = graphMapper->FindNextPositions(grid);

            if (nextPositions.size() == 1) {
                graphMapper->MoveTo(nextPositions[0]);
                continue;
            }
            else {
                // We're at a junction...
                if (nodes.contains(graphMapper->position)) {
                    // We've already been to this node before...
                    // Is there an edge back to where we came from?
                    auto endNode = nodes[graphMapper->position];
                    if (!endNode->IsConnectedTo(graphMapper->previousNode)) {
                        Edge* newEdge = new Edge(graphMapper->previousNode, endNode, graphMapper->distance);
                        endNode->edges.push_back(newEdge);
                        graphMapper->previousNode->edges.push_back(newEdge);
                    }

                    graphMappers.pop();
                    delete graphMapper;
                    continue;
                }
                else {
                    Node* newNode = new Node(graphMapper->position);
                    nodes.insert({newNode->position, newNode});

                    Edge* newEdge = new Edge(newNode, graphMapper->previousNode, graphMapper->distance);
                    edges.insert({ newEdge->nodeA, newEdge });
                    edges.insert({ newEdge->nodeB, newEdge });
                    newNode->edges.push_back(newEdge);
                    graphMapper->previousNode->edges.push_back(newEdge);
                    
                    for (auto nextPosition : nextPositions) {
                        GraphMapper* newGraphMapper = new GraphMapper(nextPosition, graphMapper->position, newNode, 1);
                        graphMappers.push(newGraphMapper);
                    }

                    graphMappers.pop();
                    delete graphMapper;
                }
            }
        }
    }
};