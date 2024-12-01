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

struct Range {
public:
    int min = 1;
    int max = 4000;

    bool IsValueWithinRange(int value) {
        return value >= min && value <= max;
    }

    std::tuple<Range, Range> Split(char comparison, int splitValue) {
        if (comparison == '<') {
            Range acceptedRange{ min, splitValue - 1 };
            Range rejectedRange{ splitValue, max };

            return { acceptedRange, rejectedRange };

        }
        else if (comparison == '>') {
            Range rejectedRange{ min, splitValue };
            Range acceptedRange{ splitValue + 1, max };

            return { acceptedRange, rejectedRange };
        }
        else {
            throw "Unreconized operation.";
        }
    }

    long long Permutations() {
        return (max - min) + 1;
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


struct RangeMachinePart {
public:
    Range xRange{};
    Range mRange{};
    Range aRange{};
    Range sRange{};  

    Range& GetAffectedRange(const Rule& rule) {
        Range affectedRange;
        switch (rule.targetProperty) {
        case 'x': return xRange;
        case 'm': return mRange;
        case 'a': return aRange;
        case 's': return sRange; 
        };

    }

    bool DoesRuleSplitPart(const Rule& rule) {
        bool a = rule.destination == "A";
        bool b = rule.destination == "R";
        bool c = rule.targetProperty == ' ';
        bool d = rule.comparison == ' ';
        if (rule.targetProperty == ' ' || rule.comparison == ' ')
            return false;

        Range& affectedRange = GetAffectedRange(rule);
        return rule.value > affectedRange.min && rule.value < affectedRange.max;
    }

    std::tuple<RangeMachinePart, RangeMachinePart> SplitPartOnRule(const Rule& rule) {        
        switch (rule.targetProperty) {
        case 'x': {
            auto subRanges = xRange.Split(rule.comparison, rule.value);
            RangeMachinePart acceptedSubPart{ std::get<0>(subRanges), mRange, aRange, sRange };
            RangeMachinePart rejectedSubPart{ std::get<1>(subRanges), mRange, aRange, sRange };
            return { acceptedSubPart, rejectedSubPart };
        }
        case 'm': {
            auto subRanges = mRange.Split(rule.comparison, rule.value);
            RangeMachinePart acceptedSubPart{ xRange, std::get<0>(subRanges), aRange, sRange };
            RangeMachinePart rejectedSubPart{ xRange, std::get<1>(subRanges), aRange, sRange };
            return { acceptedSubPart, rejectedSubPart };
        }
        case 'a': {
            auto subRanges = aRange.Split(rule.comparison, rule.value);
            RangeMachinePart acceptedSubPart{ xRange, mRange, std::get<0>(subRanges), sRange };
            RangeMachinePart rejectedSubPart{ xRange, mRange, std::get<1>(subRanges), sRange };
            return { acceptedSubPart, rejectedSubPart };
        }
        case 's': {
            auto subRanges = sRange.Split(rule.comparison, rule.value);
            RangeMachinePart acceptedSubPart{ xRange, mRange, aRange, std::get<0>(subRanges) };
            RangeMachinePart rejectedSubPart{ xRange, mRange, aRange, std::get<1>(subRanges) };
            return { acceptedSubPart, rejectedSubPart };
        }
        default:
            throw "Unrecognized target property: " + rule.targetProperty;
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

std::tuple<std::vector<RangeMachinePart>, std::vector<RangeMachinePart>> ProcessRangePartThroughWorkflow(RangeMachinePart& rangePart, const Workflow& workflow, const std::map<std::string,Workflow>& allWorkflows) {

    std::vector<RangeMachinePart> acceptedParts;
    std::vector<RangeMachinePart> rejectedParts;
    RangeMachinePart targetRangePart = rangePart;
    for (auto& rule : workflow.rules) {
        if (targetRangePart.DoesRuleSplitPart(rule)) {
            auto splitResult = targetRangePart.SplitPartOnRule(rule);
            RangeMachinePart triggeredPart = std::get<0>(splitResult);
            if (rule.destination == "A") {
                acceptedParts.push_back(triggeredPart);
            }
            else if (rule.destination == "R") {
                rejectedParts.push_back(triggeredPart);
            }
            else {
                std::string triggeredDestination = rule.destination;
                Workflow nextWorkflow = allWorkflows.find(triggeredDestination)->second;
                auto subResult = ProcessRangePartThroughWorkflow(triggeredPart, nextWorkflow, allWorkflows);
                for(auto& acceptedPart : std::get<0>(subResult))
                    acceptedParts.push_back(acceptedPart);

                for (auto& rejectedPart : std::get<1>(subResult))
                    rejectedParts.push_back(rejectedPart);                
            }

            RangeMachinePart ignoredPart = std::get<1>(splitResult);
            targetRangePart = ignoredPart;
        }
        else {
            if (rule.destination == "A") {
                acceptedParts.push_back(targetRangePart);
            }
            else if (rule.destination == "R") {
                rejectedParts.push_back(targetRangePart);
            }
            else {
                auto& nextWorkflow = allWorkflows.find(rule.destination)->second;
                auto subResult = ProcessRangePartThroughWorkflow(targetRangePart, nextWorkflow, allWorkflows);
                for (auto& acceptedPart : std::get<0>(subResult))
                    acceptedParts.push_back(acceptedPart);
                for (auto& rejectedPart : std::get<1>(subResult))
                    rejectedParts.push_back(rejectedPart);
            }
        }
    }

    return { acceptedParts, rejectedParts };
}

void DoPart2(const std::map<std::string, Workflow>& workflows) {

    RangeMachinePart initialPart;   
    Workflow initialWorkflow = workflows.find("in")->second;
    std::tuple<std::vector<RangeMachinePart>, std::vector<RangeMachinePart>> partClassifications = ProcessRangePartThroughWorkflow(initialPart, initialWorkflow, workflows);

    auto acceptedRangeParts = std::get<0>(partClassifications);    
    long long totalAcceptablePartPermutations = 0;
    for (auto& part : acceptedRangeParts) {
        long long xPerms = part.xRange.Permutations();
        long long mPerms = part.mRange.Permutations();
        long long aPerms = part.aRange.Permutations();
        long long sPerms = part.sRange.Permutations();
        long long permutations = xPerms * mPerms * aPerms * sPerms;
        totalAcceptablePartPermutations += permutations;        
    }

    std::cout << "PART 2 ANSWER - Total combination of valid parts: " << totalAcceptablePartPermutations << "\n";
}

int main()
{
    std::cout << "Advent of Code - Day 19!\n";

    auto input = ParseInput("input.txt");
    auto workflows = std::get<0>(input);
    auto machineParts = std::get<1>(input);

    DoPart1(workflows, machineParts);    
    DoPart2(workflows);
    return 0;
}