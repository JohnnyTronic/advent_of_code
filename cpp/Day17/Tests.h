#pragma once

#include "Node.h"
#include "NodeCostCompare.h"
#include "Grid.h"
#include <queue>
#include <cassert>


void TestPriorityQueue() {
    auto frontier = std::priority_queue<Node*, std::vector<Node*>, NodeCostCompare>();

    Node* nodeA = new Node(Vec2(1, 1), nullptr, 0, 41);
    Node* nodeB = new Node(Vec2(2, 3), nullptr, 0, 600);
    Node* nodeC = new Node(Vec2(4, 5), nullptr, 0, 30);
    Node* nodeD = new Node(Vec2(6, 7), nullptr, 0, 40);
    Node* nodeE = new Node(Vec2(8, 9), nullptr, 0, 20);
    Node* nodeF = new Node(Vec2(10, 11), nullptr, 0, 10);

    frontier.push(nodeA);
    frontier.push(nodeB);
    frontier.push(nodeC);
    frontier.push(nodeD);
    frontier.push(nodeE);
    frontier.push(nodeF);

    assert(frontier.top()->costSoFar == 10 && "Top value should initially be 10.");
    assert(frontier.top()->costSoFar == 10 && "Without popping, top value should still be 10.");
    frontier.pop();
    assert(frontier.top()->costSoFar == 20);
    frontier.pop();
    assert(frontier.top()->costSoFar == 30);
    frontier.pop();
    assert(frontier.top()->costSoFar == 40);
    frontier.pop();
    assert(frontier.top()->costSoFar == 41);
    frontier.pop();
    assert(frontier.top()->costSoFar == 600);
    frontier.pop();
    assert(frontier.empty());

    std::cout << "Test Priority Queue: SUCCESS\n";
}

void TestNodeHashing() {
    Node* nodeA = new Node(Vec2(1, 1), nullptr, 0, 41);
    Node* nodeB = new Node(Vec2(2, 3), nodeA, 0, 600);
    Node* nodeC = new Node(Vec2(4, 5), nodeB, 0, 30);
    Node* nodeD = new Node(Vec2(6, 7), nodeC, 0, 40);
    Node* nodeE = new Node(Vec2(8, 9), nodeD, 0, 20);
    Node* nodeF = new Node(Vec2(10, 11), nodeE, 2, 10);
    Node* nodeG = new Node(Vec2(10, 11), nodeE, 2, 11);
    Node* nodeH = new Node(Vec2(11, 11), nodeE, 2, 10);
    Node* nodeI = new Node(Vec2(10, 12), nodeE, 2, 10);
    Node* nodeJ = new Node(Vec2(10, 11), nodeE, 3, 10);
    
    std::vector<Node*> nodes{ nodeA,nodeB,nodeC,nodeD,nodeE,nodeF,nodeG,nodeH,nodeI,nodeJ};
    for (int i = 0; i < nodes.size() - 1; i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            assert(nodes[i]->Hash() != nodes[j]->Hash() && "Hash collision");
        }
    }

    std::cout << "Test Node Hashing: SUCCESS\n";
}

bool TestDirectionsHelper(Vec2 given, Vec2 expectedA, Vec2 expectedB, Vec2 expectedC) {
    auto results = GetPossibleNextDirections(given);
    assert(results.size() == 3);
    assert(std::find(results.begin(), results.end(), expectedA) != results.end());
    assert(std::find(results.begin(), results.end(), expectedB) != results.end());
    assert(std::find(results.begin(), results.end(), expectedC) != results.end());
    return true;
}

void TestGetPossibleNextDirections() {
    assert(TestDirectionsHelper(NORTH, WEST, NORTH, EAST));
    assert(TestDirectionsHelper(SOUTH, WEST, SOUTH, EAST));
    assert(TestDirectionsHelper(EAST, SOUTH, NORTH, EAST));
    assert(TestDirectionsHelper(WEST, WEST, NORTH, SOUTH));
 
    std::cout << "Test GetPossibleNextDirections: SUCCESS\n";
}

void AssertXInY(Vec2 expected, std::vector<Vec2> container) {
    assert(std::find(container.begin(), container.end(), expected) != container.end());
}

void TestGetValidNeighbourPositions() {
    std::vector<std::string> input{ "1234","1234","1234", "1234" };
    Grid<int> grid(input);
    {
        Node* nodeA = new Node(Vec2(0, 0), nullptr, 0, 0);
        auto result = GetValidNeighboursPositions(nodeA, grid);
        assert(result.size() == 2);
        assert(std::find(result.begin(), result.end(), Vec2(1, 0)) != result.end());
        assert(std::find(result.begin(), result.end(), Vec2(0, 1)) != result.end());
    }

    {
        Node* nodeB = new Node(Vec2(1, 1), nullptr, 0, 0);
        auto result = GetValidNeighboursPositions(nodeB, grid);
        assert(result.size() == 4);
        AssertXInY(Vec2(1, 0), result);
        AssertXInY(Vec2(0, 1), result);
        AssertXInY(Vec2(2, 1), result);
        AssertXInY(Vec2(1, 2), result);
    }
    {
        Node* nodeC = new Node(Vec2(0, 1), nullptr, 0, 0);
        Node* nodeD = new Node(Vec2(1, 1), nodeC, 1, 10);
        auto result = GetValidNeighboursPositions(nodeD, grid);
        assert(result.size() == 3);
        AssertXInY(Vec2(2, 1), result);
        AssertXInY(Vec2(1, 0), result);
        AssertXInY(Vec2(1, 2), result);
    }
    {
        // Steps=3 prevent further forward movement
        Node* nodeE = new Node(Vec2(0, 1), nullptr, 0, 0);
        Node* nodeF = new Node(Vec2(1, 1), nodeE, 3, 0);
        auto result = GetValidNeighboursPositions(nodeF, grid);
        assert(result.size() == 2);
        AssertXInY(Vec2(1, 0), result);
        AssertXInY(Vec2(1, 2), result);
    }
    {
        // Steps=2 does NOT prevent further forward movement
        Node* nodeG = new Node(Vec2(0, 1), nullptr, 0, 0);
        Node* nodeH = new Node(Vec2(1, 1), nodeG, 2, 0);
        auto result = GetValidNeighboursPositions(nodeH, grid);
        assert(result.size() == 3);
        AssertXInY(Vec2(1, 0), result);
        AssertXInY(Vec2(1, 2), result);
        AssertXInY(Vec2(2, 1), result);
    }
    {
        Node* nodeI = new Node(Vec2(3, 3), nullptr, 0, 0);
        auto result = GetValidNeighboursPositions(nodeI, grid);
        assert(result.size() == 2);
    }

    std::cout << "Test GetValidNeighbourPositions: SUCCESS\n";
}