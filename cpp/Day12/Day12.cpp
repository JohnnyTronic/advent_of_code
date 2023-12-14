// Solution adapted from https://www.reddit.com/r/adventofcode/comments/18hbbxe/2023_day_12python_stepbystep_tutorial_with_bonus/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <set>
#include <regex>
#include <map>
#include <thread>
#include <semaphore>
#include <mutex>

static std::map<std::string, long long> memos{};

long long calc(std::string record, std::vector<int> groups);

// Logic that treats the first character as a #
long long pound(std::string record, int nextGroup, std::vector<int> groups) {

    // If the first is a pound, then the first n characters must be able to be treated as a pound, hwere n is the first group number
    std::string thisGroup = record.substr(0, nextGroup);
    int qIndex = thisGroup.find('?');
    while (qIndex != std::string::npos) {
        thisGroup.replace(qIndex, 1, "#");
        qIndex = thisGroup.find('?');
    }

    // If the next group can't fit all the damaged springs, then abort
    std::string comparisonGroup = "";
    for (int i = 0; i < nextGroup; i++)
        comparisonGroup += "#";
    if (thisGroup != comparisonGroup) {
        return 0;
    }

    // If the rest of the record is just the last group, then we're done and there's only one possibility
    if (record.size() == nextGroup) {
        // Make sure this is the last group
        if (groups.size() == 1) {
            // We are valid
            return 1;
        }
        else {
            // There's no more groups, we can't make it work
            return 0;
        }
    }

    // Make sure the character that follows this group can be a separator
    if (record[nextGroup] == '?' || record[nextGroup] == '.') {
        // It can be a separator, so skip it and reduce to the next group
        groups.erase(groups.begin());
        return calc(record.substr(nextGroup + 1), groups);
    }

    // Can't be handled, there are no possibilities
    return 0;
}

// Logic that treats the first character as a dot
long long dot(std::string record, std::vector<int> groups) {

    // We just skip over the dot looking for the next pound
    record.erase(record.begin());
    return calc(record, groups);
}

long long calc(std::string record, std::vector<int> groups) {

    std::string groupsString = "";
    bool toRemoveComma = false;
    for (auto group : groups) {
        groupsString += std::to_string(group) + ',';
        toRemoveComma = true;
    }
    if(toRemoveComma)
        groupsString.pop_back();   
    
    std::string memoString = record + " " + groupsString;
    if (memos.contains(memoString)) {
        return memos[memoString];
    }

    long long out{};

    // Did we run out of groups? We might still be valid
    if (groups.size() == 0) {

        // Make sure there aren't any more damaged springs, if so, we're valid
        if (record.find('#') == std::string::npos) {
            // This will return true even if record is empty, which is valid
            return 1;
        }
        else {
            // More damaged springs that we can't fit
            return 0;
        }
    }

    // There are more groups but not more records
    if (record.size() == 0) {
        // We can't fit, exit
        return 0;
    }

    // Look at the next element in each record and group
    char nextCharacter = record[0];
    int nextGroup = groups[0];

    if (nextCharacter == '#') {
        out = pound(record, nextGroup, groups);
    } else if (nextCharacter == '.') {
        out = dot(record, groups);
    }
    else if (nextCharacter == '?') {
        // This character could be either character, so we're explore both possibilities
        out = dot(record, groups) + pound(record, nextGroup, groups);
    }
    else
        throw;

    memos[memoString] = out;
    return out;    
}

long long DoPart2() {
    long long totalPossibilities = 0;

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    std::string parsedLine;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);

        int spaceIndex = parsedLine.find(' ');
        std::string record = parsedLine.substr(0, spaceIndex);
        std::string rawGroups = parsedLine.substr(spaceIndex + 1);

        std::vector<int> groups{};
        int commaIndex = rawGroups.find(',');
        while (commaIndex != std::string::npos) {
            groups.push_back(std::stoi(rawGroups.substr(0, commaIndex)));
            rawGroups = rawGroups.substr(commaIndex + 1);
            commaIndex = rawGroups.find(',');
        }
        groups.push_back(std::stoi(rawGroups));

        std::string xRecord = record;
        std::vector<int> xGroups = groups;
        for (int r = 0; r < 4; r++) {
            xRecord += "?" + record;
            xGroups.insert(xGroups.begin(), groups.begin(), groups.end());
        }

        totalPossibilities += calc(xRecord, xGroups);
    }

    std::cout << "PART 2 ANSWER - Total possibilities for expanded instruction set: " << totalPossibilities << "\n";

    return totalPossibilities;
}



int main()
{
    std::cout << "Advent of Code - Day 12\n";

    long long totalPossibilities{};

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");
      
    std::string parsedLine; 
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        
        int spaceIndex = parsedLine.find(' ');
        std::string record = parsedLine.substr(0, spaceIndex);
        std::string rawGroups = parsedLine.substr(spaceIndex + 1);

        std::vector<int> groups{};
        int commaIndex = rawGroups.find(',');
        while (commaIndex != std::string::npos) {
            groups.push_back(std::stoi(rawGroups.substr(0, commaIndex)));
            rawGroups = rawGroups.substr(commaIndex + 1);
            commaIndex = rawGroups.find(',');
        }
        groups.push_back(std::stoi(rawGroups));

        totalPossibilities += calc(record, groups);
    }

    std::cout << "PART 1 ANSWER - Total possible arrangements: " << totalPossibilities << "\n";

    // Part 2 - Unfold the damned instructions!
    DoPart2();
    
    return 0;
}
