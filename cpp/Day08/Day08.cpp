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
    std::string leftString;
    std::string rightString;

    Node* leftNode;
    Node* rightNode;

    Node() {}

    Node(std::string value, std::string left, std::string right) : value(value), leftString(left), rightString(right) {}

    void SetUpNodeReferences(std::map<std::string, Node*>& nodeMap) {
        leftNode = nodeMap[leftString];
        rightNode = nodeMap[rightString];
    }
};

//class NodeMap {
//public:
//    std::map<std::string, Node> nodes;
//};

class Ghost {
public:
    std::string startingNodeString;
    Node* currentNode;
    //NodeMap& nodeMap;
    Instructions instructions;   

    Ghost(Node* startingNode, Instructions instructions) : currentNode(startingNode), instructions(instructions) {
        startingNodeString = startingNode->value;       
    }

    void Step() {
        char stepInstruction = this->instructions.GetNextStep();
        if (stepInstruction == 'L') {
            //std::string nextNode = currentNode.left;
            //currentNode = nodeMap.nodes[nextNode];
            currentNode = currentNode->leftNode;
            //totalSteps++;
        }
        else {
            //std::string nextNode = currentNode.right;
            //currentNode = nodeMap.nodes[nextNode];
            currentNode = currentNode->rightNode;
            //totalSteps++;
        }       
    }

    bool doesCurrentNodeEndWithZ() {
        return currentNode->value[2] == 'Z';
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
    std::map<std::string, Node*> nodeMap;
    std::string mapLine;
    std::getline(ifs, mapLine);
    std::regex nodeLinePattern(R"((\w\w\w) = \((\w\w\w), (\w\w\w))");
    while (std::getline(ifs, mapLine)) {
        std::smatch match;
        bool matchSuccess = std::regex_search(mapLine, match, nodeLinePattern);
        if (!matchSuccess)
            std::cout << "Regex failure!!!" << std::endl;

        Node* newNode = new Node(match[1], match[2], match[3]);
        nodeMap[match[1]] = newNode;
    }

    for (auto kvp : nodeMap) {
        kvp.second->SetUpNodeReferences(nodeMap);
    }

    // Start walking the node map
    Node* currentNode = nodeMap["AAA"];
    int totalSteps = 0;
    while (currentNode->value != "ZZZ") {
        char nextStep = instructions.GetNextStep();
        if (nextStep == 'L') {
            //std::string nextNode = currentNode.left;
            //currentNode = nodeMap.nodes[nextNode];
            currentNode = currentNode->leftNode;
            totalSteps++;
        }
        else {
            //std::string nextNode = currentNode.right;
            //currentNode = nodeMap.nodes[nextNode];
            currentNode = currentNode->rightNode;
            totalSteps++;
        }
    }

    std::cout << "PART 1 ANSWER - Total step count: " << totalSteps << std::endl;

    // Part 2 - Simultaneous Ghost pathing
    instructions.Reset();

    std::vector<Node*> ghostStartingNodes;
    for (auto& kvp : nodeMap) {
        if (kvp.first[2] == 'A')
            ghostStartingNodes.push_back(kvp.second);
    }

    std::vector<Ghost> ghosts;
    std::cout << "Ghost starting nodes: " << std::endl;
    for (auto& startingNode : ghostStartingNodes) {
        std::cout << startingNode->value << std::endl;
        Ghost ghost(startingNode, instructions);
        ghosts.push_back(ghost);
    }

    std::map<std::string, int> ghostLoopSizes;
    for (auto& ghost : ghosts) {
        int stepCount = 0;
        while (!ghost.doesCurrentNodeEndWithZ()) {
            ghost.Step();
            stepCount++;
        }
        ghostLoopSizes[ghost.startingNodeString] = stepCount;
    }

    std::cout << "---Ghost Loop Sizes---" << std::endl;
    for (auto& ghost : ghosts) {
        std::cout << ghost.startingNodeString << ": " << ghostLoopSizes[ghost.startingNodeString] << std::endl;
    }
    
    // Find Least Common Multiple of all loop sizes
    std::vector<long long> startingLoopSizes;
    std::vector<long long> lcm;
    for (auto& ghostLoopSize : ghostLoopSizes) {
        startingLoopSizes.push_back(ghostLoopSize.second);
        lcm.push_back(ghostLoopSize.second);
    }
    auto areAllSame = [](std::vector<long long>& lcms) {
        long long first = lcms.front();
        for (int i = 1; i < lcms.size(); i++) {
            if (lcms[i] != first)
                return false;
        }
        return true;
    };

    auto findSmallestIndex = [](std::vector<long long>& lcms) -> int {
        int smallestIndex = 0;
        long long smallestValue = lcms[0];
        for (int i = 1; i < lcms.size(); i++) {
            if (lcms[i] < smallestValue) {
                smallestIndex = i;
                smallestValue = lcms[i];
            }
        }
        return smallestIndex;
    };

    long long targetValue = 15746133679061;
    while (!areAllSame(lcm)) {
        int smallestIndex = findSmallestIndex(lcm);
        lcm[smallestIndex] += startingLoopSizes[smallestIndex];
        if (lcm[smallestIndex] > targetValue) {
            std::cout << "We've missed our target value!" << std::endl;
            return 1;
        }
    }

    std::cout << "PART 2 ANSWER - Lowest Common Multiple is " << lcm[0] << std::endl;
    return 0;    
}