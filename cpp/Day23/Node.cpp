#include "Node.h"

bool Node::operator==(const Node& other) {
    return position.x == other.position.x && position.y == other.position.y;
}

Node* Edge::GetOther(Node* start) {
    return start == nodeA ? nodeB : nodeA;
}

bool Edge::operator==(const Edge& rhs) const {
    return nodeA->position.x == rhs.nodeA->position.x && nodeA->position.y == rhs.nodeA->position.y && nodeB->position.x == rhs.nodeB->position.x && nodeB->position.y == rhs.nodeB->position.y && distance == rhs.distance;
}