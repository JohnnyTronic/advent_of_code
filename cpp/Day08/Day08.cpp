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
}