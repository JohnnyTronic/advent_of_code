#include "Module.h"
#include "BroadcasterModule.h"
#include "FlipFlopModule.h"
#include "ConjunctionModule.h"
#include "PulseQueue.h"
#include "ButtonModule.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
#include <vector>
#include <map>

std::tuple<std::map<std::string, Module*>, PulseQueue*> ParseInput(const std::string& fileName) {

    std::map<std::string, Module*> moduleMap;
    PulseQueue* pulseQueue = new PulseQueue();

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
            std::string broadcasterModuleName = "broadcaster";
            Module* broadcasterModule = new BroadcasterModule(broadcasterModuleName, pulseQueue);
            moduleMap[broadcasterModule->name] = broadcasterModule;
        }
        else {
            std::smatch moduleNameMatch;
            auto moduleNameSearchResult = std::regex_search(moduleNameString, moduleNameMatch, moduleNamePattern);
        }        
    }

    return { moduleMap, pulseQueue };
}

void DoPart1(std::map<std::string, Module*> moduleMap, PulseQueue* pulseQueue) {
    
    Module* buttonModulePtr = moduleMap.find("button")->second;
    //ButtonModule* buttonModule { std::static_cast<ButtonModule*>(buttonModulePtr) };
    int buttonPushCount = 1000;
    for (int i = 0; i < buttonPushCount; i++) {
        //buttonModule->PushButton();
        pulseQueue->SimulatePulses();
    }


    long long productOfTotalPulses = pulseQueue->highPulseTally * pulseQueue->lowPulseTally;

    std::cout << "PART 1 ANSWER - Product of total low and high pulses: " << productOfTotalPulses << "\n";
}

void DoPart2(std::map<std::string, Module*> moduleMap, PulseQueue* pulseQueue) {
    // Not implemented yet.
    pulseQueue->Reset();
}

int main()
{
    std::cout << "Advent of Code - Day 20!\n";

    auto inputs = ParseInput("test_input.txt");
    auto moduleMap = std::get<0>(inputs);
    auto pulseQueue = std::get<1>(inputs);

    DoPart1(moduleMap, pulseQueue);
    DoPart2(moduleMap, pulseQueue);
}
