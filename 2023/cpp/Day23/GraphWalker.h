#pragma once
#include "Node.h"

struct GraphWalker {
public:
	Node* currentNode;
	std::unordered_set<Node*, NodeHashFunction> visitedNodes;
	long totalDistance{};

	GraphWalker(Node* position, std::unordered_set<Node*, NodeHashFunction> visitedNodes, long totalDistance) : currentNode(position), visitedNodes(visitedNodes), totalDistance(totalDistance) {};

	std::vector<Edge*> GetUntravelledEdges() {
		std::vector<Edge*> untravelledEdges;
		for (auto candidateEdge : currentNode->edges) {
			auto otherNode = candidateEdge->GetOther(currentNode);
			if (visitedNodes.contains(otherNode))
				continue;

			untravelledEdges.push_back(candidateEdge);
		}

		return untravelledEdges;
	}

	void TravelAlong(Edge* edge) {
		Node* farSide = edge->GetOther(currentNode);
		totalDistance += edge->distance;
		currentNode = farSide;
		visitedNodes.insert(currentNode);
	}

	GraphWalker* Clone() {
		GraphWalker* clone = new GraphWalker(currentNode, visitedNodes, totalDistance);
		return clone;
	}
};