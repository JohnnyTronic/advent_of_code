#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <regex>
#include <map>

class History {
public:
    std::string inputString;
    std::vector<long long> values;

    History(std::string inputString) : inputString(inputString) {       
        std::regex pattern(R"(-?\d+)");
        auto numbersBegin = std::sregex_iterator(inputString.begin(), inputString.end(), pattern);
        auto numbersEnd = std::sregex_iterator();
                
        for (std::sregex_iterator iter = numbersBegin; iter != numbersEnd; iter++) {
            std::smatch match = *iter;
            std::cout << match.str() << std::endl;
            values.push_back(std::stoll(match.str()));
        }        
    }     
};

bool IsAllZero(std::vector<long long> values) {
    for (auto& value : values) {
        if (value != 0)
            return false;
    }
    return true;
};

void AppendNextValue(std::vector<long long>& values) {
    std::vector<long long> deltas;
    for (int i = 0; i < values.size() - 1; i++) {
        long long delta = values[i + 1] - values[i];
        deltas.push_back(delta);
    }

    if (IsAllZero(deltas)) {
        long long tailValue = values.back();
        values.push_back(tailValue);
        return;
    }

    AppendNextValue(deltas);
    values.push_back(values.back() + deltas.back());
}

int main()
{
    std::cout << "Advent of Code 2023 - Day 09!\n";

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    // Parse each line/history
    std::vector<History> histories;
    std::string line;       
    while (ifs.good()) {
        std::getline(ifs, line);
        History newHistory(line);
        histories.push_back(newHistory);
    }

    for (auto& history : histories) {
        AppendNextValue(history.values);
    }

    long long sumOfAllTailValues = 0;
    for (auto& history : histories) {
        sumOfAllTailValues += history.values.back();
    }

    std::cout << "PART 1 ANSWER - " << sumOfAllTailValues << std::endl;
    return 0;
}
