#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <set>
#include <regex>
#include <map>

// Example Regex pattern for 3,2,1 - [\.\?]*[#\?]{3}[\.\?]+[#\?]{2}[\.\?]+[#\?]{1}[\.\?]*


std::string TransformLegendNumbersIntoLegendRegex(std::string legendNumbers) {
    std::regex numberPattern(R"(\d+)");
    auto replacedString = std::regex_replace(legendNumbers, numberPattern, R"([#\?]{$&})");

    std::regex commaPattern(R"(,)");
    replacedString = std::regex_replace(replacedString, commaPattern, R"([\.\?]+)");

    return replacedString;
}

std::regex CreateRegexForLegendNumbers(std::string legendString) {
    std::string regexString = R"([\.\?]*)";
    
    std::string legendNumbersAsRegexString = TransformLegendNumbersIntoLegendRegex(legendString);
    regexString += legendNumbersAsRegexString;

    regexString += R"([\.\?]*)";

    return std::regex(regexString);
}

std::vector<std::string> GenerateSpringPossibilities(std::string candidateString, std::regex& legendRegex) {

    std::vector<std::string> possibilities;

    // Does candidateString qualify? If not, early return kills this branch of the multiverse
    bool overallMatch = std::regex_match(candidateString, legendRegex);
    if (!overallMatch)
        return possibilities;

    // Does candidateString contain at least one '?'
    auto position = candidateString.find('?');
    if (position == std::string::npos) {
        possibilities.push_back(candidateString);
        return possibilities;
    }

    auto multiverseCandidateA = candidateString.replace(position, 1, "#");
    auto multiverseAResult = GenerateSpringPossibilities(multiverseCandidateA, legendRegex);
    if (multiverseAResult.size() > 0) {
        possibilities.insert(possibilities.end(), multiverseAResult.begin(), multiverseAResult.end());
    }

    auto multiverseCandidateB = candidateString.replace(position, 1, ".");
    auto multiverseBResult = GenerateSpringPossibilities(multiverseCandidateB, legendRegex);
    if (multiverseBResult.size() > 0) {
        possibilities.insert(possibilities.end(), multiverseBResult.begin(), multiverseBResult.end());
    }

    return possibilities;
}

int main()
{
    std::cout << "Advent of Code - Day 12\n";

    long long totalPossibilities{};

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    std::regex springsChunkPattern(R"([\?.#]+(?=\s))");
    std::regex legendChunkPattern(R"(\s[,\d]+)");
    std::regex legendNumberPattern(R"((\d+))");
    std::string parsedLine;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        
        std::smatch springsChunkMatch;
        std::regex_search(parsedLine, springsChunkMatch, springsChunkPattern);
        std::string springsChunk = springsChunkMatch.str();
        
        std::smatch legendChunkMatch;
        std::regex_search(parsedLine, legendChunkMatch, legendChunkPattern);

        std::string legendChunk = legendChunkMatch.str();
        std::regex spaceChopperPattern(R"(^\s*)");
        legendChunk = std::regex_replace(legendChunk, spaceChopperPattern, "");
        
        auto legendRegex = CreateRegexForLegendNumbers(legendChunk);

        // Sanity check: does legendRegex match against the root springs chunk?
        bool didMatch = std::regex_match(springsChunk, legendRegex);
        if (!didMatch)
            throw;

        auto possibilities = GenerateSpringPossibilities(springsChunk, legendRegex);
        totalPossibilities += possibilities.size();
    }

    std::cout << "PART 1 ANSWER - Total possible arrangements: " << totalPossibilities << "\n";

    return 0;
}
