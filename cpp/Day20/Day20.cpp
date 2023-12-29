#include "Module.h"
#include "BroadcasterModule.h"
#include "FlipFlopModule.h"
#include "ConjunctionModule.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
#include <vector>
#include <map>

std::map<std::string, Module*> ParseInput(const std::string& fileName) {

    std::map<std::string, Module*> moduleMap;
    PulseQueue pulseQueue;

    std::ifstream ifs(fileName);
    std::string parsedLine;    
    std::regex wholeModulePattern(R"((.+)\s->\s(.+))");
    std::regex moduleNamePattern(R"(([%&])?(\w+))");
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        std::smatch wholeModuleMatch;
        auto searchResult = std::regex_search(parsedLine, wholeModuleMatch, wholeModulePattern);
        if (!searchResult)
            throw "Bad regex search result.";

        auto moduleNameString = wholeModuleMatch[1].str();
        auto downstreamModulesString = wholeModuleMatch[2].str();

        if (moduleNameString == "broadcaster") {
            Module* broadcasterModule = new BroadcasterModule("broadcaster", pulseQueue);
            moduleMap[broadcasterModule->name] = broadcasterModule;
        }
        else {
            std::smatch moduleNameMatch;
            auto moduleNameSearchResult = std::regex_search(moduleNameString, moduleNameMatch, moduleNamePattern);
        }        
    }
}

void DoPart1() {
    long long productOfTotalPulses = 0;

    std::cout << "PART 1 ANSWER - Product of total low and high pulses: " << productOfTotalPulses << "\n";
}

void DoPart2() {
    // Not implemented yet.
}

int main()
{
    std::cout << "Advent of Code - Day 20!\n";

    ParseInput("test_input.txt");

    DoPart1();
    DoPart2();
}
