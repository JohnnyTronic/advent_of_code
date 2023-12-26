#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <map>

const std::string ruleNoMatchString = " ";

class MachinePart {
public:
    int x;
    int m;
    int a;
    int s;
        
    MachinePart(std::string partString) {
        // Example machine part string: {x=61,m=577,a=1750,s=1892}
        std::regex numbersPattern("=(\\d+),.*=(\\d+),.*=(\\d+),.*=(\\d+)");
        std::sregex_token_iterator iterator(partString.begin(), partString.end(), numbersPattern);
        std::smatch match;
        std::regex_search(partString, match, numbersPattern);
        x = std::stoi(match[1]);
        m = std::stoi(match[2]);
        a = std::stoi(match[3]);
        s = std::stoi(match[4]);
    }
};

class Rule {
public:
    char targetProperty = ' ';
    char comparison = ' ';
    long long value = 0;
    std::string destination;

    Rule(std::string destination) :destination(destination) {}

    Rule(char targetProperty, char comparison, long long value, std::string destination) : targetProperty(targetProperty), comparison(comparison), value(value), destination(destination) {}

    std::string EvaluateMachinePart(const MachinePart& part) {
        // Does this part trigger this rule? Return "destination" if so, return " " if not
        if (comparison == ' ') {
            return destination;
        }

        int partValue = -1;
        switch (targetProperty) {        
        case 'x': partValue = part.x; break;
        case 'm': partValue = part.m; break;
        case 'a': partValue = part.a; break;
        case 's': partValue = part.s; break;
        }
        if (comparison == '<') {
            if (partValue < value)
                return destination;
            else
                return ruleNoMatchString;
        }
        else if (comparison == '>') {
            if (partValue > value)
                return destination;
            else
                return ruleNoMatchString;
        }
        else {
            throw "Unrecognized comparison char: " + comparison;
        }
    }
};

class Workflow {
public:
    std::string label{};
    std::vector<Rule> rules{};

    Workflow(){}

    Workflow(std::string label, std::string rulesString) : label(label) {
        // Parse rules string into individual rules
        // Example rules string: s<3181:R,R
        std::regex rulePattern(R"(([xmas][<>]\d+)?:?(\w+))");
        std::regex conditionPattern(R"(([xmas])([<>])(\d+))");
        int captureGroupTargets[] = { 1,2 };
        std::sregex_token_iterator iterator(rulesString.begin(), rulesString.end(), rulePattern);
        std::sregex_token_iterator endIterator;
        while (iterator != endIterator) {
            auto ruleString = (*iterator).str();      

           std::smatch submatch;
           std::regex_search(ruleString, submatch, rulePattern);
           std::string rawCondition = submatch[1];
           std::string destination = submatch[2];

           if (rawCondition == "") {
            Rule newRule(destination);
            rules.push_back(newRule);
           }
           else {
               std::smatch conditionSubmatches;
               std::regex_search(rawCondition, conditionSubmatches, conditionPattern);
               std::string targetProperty = conditionSubmatches[1];
               std::string operation = conditionSubmatches[2];
               std::string value = conditionSubmatches[3];
               Rule newRule(targetProperty[0], operation[0], std::stoll(value), destination);
               rules.push_back(newRule);
           }

           iterator++;       
        }       
    }

    std::string Evaluate(MachinePart& part) {
        for (auto rule : rules) {
            std::string result = rule.EvaluateMachinePart(part);
            if (result == ruleNoMatchString) {
                continue;
            }
            else {
                return result;
            }
        }
    }
};

enum class ParseMode {
    workflow,
    machinepart
};

std::tuple<std::map<std::string, Workflow>, std::vector<MachinePart>> ParseInput(const std::string& fileName) {
    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedWorkflowStrings;
    std::vector<std::string> parsedPartStrings;
    ParseMode parseMode = ParseMode::workflow;

    std::map<std::string, Workflow> workflows;
    std::vector<MachinePart> machineParts;
    std::regex workflowPatternOne(R"(^(\w+)\{(.*)\}$)");
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        if (parseMode == ParseMode::workflow) {

            if (parsedLine.empty()) {
                parseMode = ParseMode::machinepart;
                continue;
            }

            // Example workflow string: rb{x>1585:cs,x<1567:bbc,m>1281:nvr,R}
            std::smatch match;
            std::regex_search(parsedLine, match, workflowPatternOne);
            Workflow newWorkflow(match[1], match[2]);
            workflows[newWorkflow.label] = newWorkflow;
        }
        else {
            // Example part string: {x=1693,m=696,a=2186,s=138}
            MachinePart newPart(parsedLine);
            machineParts.push_back(newPart);
        }
    }
    ifs.close();

    return { workflows, machineParts };
}

void DoPart1(const std::map<std::string,Workflow>& workflows, const std::vector<MachinePart>& machineParts) {
    long long totalScore = 0;
    for (auto part : machineParts) {        
        Workflow workflow = workflows.find("in")->second;
        std::string destination = " ";

        destination = workflow.Evaluate(part);
        while (!(destination == "R" || destination == "A")) {
            workflow = workflows.find(destination)->second;
            destination = workflow.Evaluate(part);
        }

        if (destination == "A") {
            totalScore += part.x + part.m + part.a + part.s;
        }
    }

    std::cout << "PART 1 ANSWER - Sum of all accepted part values: " << totalScore << "\n";
}

int main()
{
    std::cout << "Advent of Code - Day 19!\n";

    auto input = ParseInput("input.txt");
    auto workflows = std::get<0>(input);
    auto machineParts = std::get<1>(input);

    DoPart1(workflows, machineParts);    
        
    return 0;
}