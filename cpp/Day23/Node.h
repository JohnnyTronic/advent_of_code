#pragma once
#include "Vec2.h"
//#include "Edge.h"
#include <vector>

struct Edge;

struct Node {
    Vec2 position;
    std::vector<Edge*> edges;

    Node(Vec2 position) : position(position) {};

    bool operator==(const Node& other);    
};

struct Edge {
public:
    Node* nodeA;
    Node* nodeB;
    int distance;

    Edge(Node* a, Node* b, int distance) : nodeA(a), nodeB(b), distance(distance) {};

    Node* GetOther(Node* start);

    bool operator==(const Edge& rhs) const;
};