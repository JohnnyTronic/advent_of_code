#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <regex>
#include <map>

class Instructions {
public:
    int currentIndex = 0;
    Instructions(std::string instructionLine) : instructionLine(instructionLine) {

    }

    char GetNextStep() {
        char toReturn = instructionLine[currentIndex];
        currentIndex++;
        if (currentIndex >= instructionLine.length())
            currentIndex = 0;

        return toReturn;
    }

    void Reset() {
        currentIndex = 0;
    }

private:
    std::string instructionLine;
};

class Node {
public:
    std::string value;
    std::string left;
    std::string right;
    Node() {}

    Node(std::string value, std::string left, std::string right) : value(value), left(left), right(right) {}
};

class NodeMap {
public:
    std::map<std::string, Node> nodes;
};

class Ghost {
public:
    std::string startingNodeString;
    Node& currentNode;
    NodeMap& nodeMap;
    Instructions instructions;   

    Ghost(Node& startingNode, NodeMap& nodeMap, Instructions instructions) : currentNode(startingNode), nodeMap(nodeMap), instructions(instructions) {
        startingNodeString = startingNode.value;       
    }

    void Step() {
        char stepInstruction = this->instructions.GetNextStep();
        if (stepInstruction == 'L') {
            std::string nextNode = currentNode.left;
            currentNode = nodeMap.nodes[nextNode];
            //totalSteps++;
        }
        else {
            std::string nextNode = currentNode.right;
            currentNode = nodeMap.nodes[nextNode];
            //totalSteps++;
        }       
    }

    bool doesCurrentNodeEndWithZ() {
        return currentNode.value[2] == 'Z';
    }   
};

int main()
{
    std::cout << "Advent of Code 2023 - Day 08!\n";

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    // Parse instructions
    std::string instructionsString;         
    std::getline(ifs, instructionsString); // Instruction line
    Instructions instructions(instructionsString);

    // Parse node map
    NodeMap nodeMap;
    std::string mapLine;
    std::getline(ifs, mapLine);
    std::regex nodeLinePattern(R"((\w\w\w) = \((\w\w\w), (\w\w\w))");
    while (std::getline(ifs, mapLine)) {
        std::smatch match;
        bool matchSuccess = std::regex_search(mapLine, match, nodeLinePattern);
        if (!matchSuccess)
            std::cout << "Regex failure!!!" << std::endl;

        Node newNode(match[1], match[2], match[3]);
        nodeMap.nodes[match[1]] = newNode;
    }

    // Start walking the node map
    Node currentNode = nodeMap.nodes["AAA"];
    int totalSteps = 0;
    while (currentNode.value != "ZZZ") {
        char nextStep = instructions.GetNextStep();
        if (nextStep == 'L') {
            std::string nextNode = currentNode.left;
            currentNode = nodeMap.nodes[nextNode];
            totalSteps++;
        }
        else {
            std::string nextNode = currentNode.right;
            currentNode = nodeMap.nodes[nextNode];
            totalSteps++;
        }
    }

    std::cout << "PART 1 ANSWER - Total step count: " << totalSteps << std::endl;

    // Part 2 - Simultaneous Ghost pathing
    instructions.Reset();

    std::vector<std::string> ghostStartingNodes;
    for (auto& node : nodeMap.nodes) {
        if (node.first[2] == 'A')
            ghostStartingNodes.push_back(node.first);
    }

    std::vector<Ghost> ghosts;
    std::cout << "Ghost starting nodes: " << std::endl;
    for (auto& startingNodeString : ghostStartingNodes) {
        std::cout << startingNodeString << std::endl;
        Node& startingNode = nodeMap.nodes[startingNodeString];
        Ghost ghost(startingNode, nodeMap, instructions);
        ghosts.push_back(ghost);
    }

    // Step all ghosts simultaneously until the all land on a ZNode
    int allZStepCount = 0;
    bool allOnZ = false;
    while (!allOnZ) {
        char nextInstruction = instructions.GetNextStep();
        for (auto& ghost : ghosts) {
            ghost.Step();
        }
        allZStepCount++;

        allOnZ = true;
        for (auto& ghost : ghosts) {
            if (!ghost.doesCurrentNodeEndWithZ()) {
                allOnZ = false;
                //break;
            }
            else {
                //std::cout << "Ghost " << ghost.startingNodeString << " landed on ZNode " << ghost.currentNode.value << " after " << allZStepCount << " steps." << std::endl;
            }
        }
    }

    std::cout << "PART B ANSWER - Number of steps until all ghosts are on ZNodes simultaneously: " << allZStepCount << std::endl;
}