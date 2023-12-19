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

    //MachinePart(int x, int m, int a, int s) : x(x), m(m), a(a), s(s) {}
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

    std::string EvaluateMachinePart(MachinePart& part) {
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

int main()
{
    std::cout << "Advent of Code - Day 19!\n";

    std::ifstream ifs("input.txt");
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

    long long totalScore = 0;
    for (auto part : machineParts) {
        Workflow workflow = workflows["in"];
        std::string destination = " ";

        destination = workflow.Evaluate(part);
        while (!(destination == "R" || destination == "A")) {
            workflow = workflows[destination];
            destination = workflow.Evaluate(part);
        }

        if (destination == "A") {
            totalScore += part.x + part.m + part.a + part.s;
        }        
    }

    std::cout << "PART 1 ANSWER - Sum of all accepted part values: " << totalScore << "\n";
        
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
