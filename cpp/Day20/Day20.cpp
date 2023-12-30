#include "Module.h"
#include "BroadcasterModule.h"
#include "FlipFlopModule.h"
#include "ConjunctionModule.h"
#include "PulseQueue.h"
#include "ButtonModule.h"
#include "ModuleMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <queue>
#include <vector>
#include <map>
#include <memory>

std::tuple<ModuleMap*, PulseQueue*> ParseInput(const std::string& fileName) {

    ModuleMap* moduleMap = new ModuleMap();
    PulseQueue* pulseQueue = new PulseQueue();

    std::map<Module*, std::string> downstreamModuleStrings;
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
            auto broadcasterModule = new BroadcasterModule(broadcasterModuleName, pulseQueue);
            moduleMap->AddModule(broadcasterModule);
            downstreamModuleStrings[broadcasterModule] = downstreamModulesString; // Will be used in 2nd pass to wire modules together
        }
        else {
            std::smatch moduleNameMatch;
            auto moduleNameSearchResult = std::regex_search(moduleNameString, moduleNameMatch, moduleNamePattern);
            auto moduleType = moduleNameMatch[1].str();
            auto moduleName = moduleNameMatch[2].str();
            if (moduleType == "%") {
                FlipFlopModule* flipFlopModule = new FlipFlopModule(moduleName, pulseQueue);
                moduleMap->AddModule(flipFlopModule);
                downstreamModuleStrings[flipFlopModule] = downstreamModulesString;
            }
            else if (moduleType == "&") {
                ConjunctionModule* conjunctionModule = new ConjunctionModule(moduleName, pulseQueue);
                moduleMap->AddModule(conjunctionModule);
                downstreamModuleStrings[conjunctionModule] = downstreamModulesString;
            }
            else {
                throw "Unrecognized module type: " + moduleType;
            }            
        }        
    }

    // Wire all the modules together
    for (auto kvp : downstreamModuleStrings) {
        auto module = kvp.first;
        auto downstreamString = kvp.second;
        int foundIndex = 0;
        do {
            foundIndex = downstreamString.find(", ");
            auto token = downstreamString.substr(0, foundIndex);
            downstreamString.erase(0, foundIndex + 2);

            auto downstreamModule = moduleMap->GetModule(token);
            // Some downstream modules are not otherwise mentioned
            if (downstreamModule == nullptr) {

            }
            module->AddDownstreamModule(downstreamModule);
            downstreamModule->AddUpstreamModule(module);
        } while (foundIndex != std::string::npos);        
    }
    
    ButtonModule* buttonModule = new ButtonModule(moduleMap->broadcasterModule, pulseQueue);
    moduleMap->AddModule(buttonModule);

    return { moduleMap, pulseQueue };
}

void DoPart1(ModuleMap* moduleMap, PulseQueue* pulseQueue) {
    
    ButtonModule* buttonModule = moduleMap->buttonModule;
    //ButtonModule* buttonModule { std::static_cast<ButtonModule*>(buttonModulePtr) };
    int buttonPushCount = 1000;
    for (int i = 0; i < buttonPushCount; i++) {
        buttonModule->PushButton();
        pulseQueue->SimulatePulses();
    }


    long long productOfTotalPulses = pulseQueue->highPulseTally * pulseQueue->lowPulseTally;

    std::cout << "PART 1 ANSWER - Product of total low and high pulses: " << productOfTotalPulses << "\n";
}

void DoPart2(ModuleMap* moduleMap, PulseQueue* pulseQueue) {
    // Not implemented yet.
    pulseQueue->Reset();
}

int main()
{
    std::cout << "Advent of Code - Day 20!\n";

    auto inputs = ParseInput("input.txt");
    auto moduleMap = std::get<0>(inputs);
    auto pulseQueue = std::get<1>(inputs);

    DoPart1(moduleMap, pulseQueue);
    DoPart2(moduleMap, pulseQueue);
}
