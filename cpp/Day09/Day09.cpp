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
    std::vector<std::vector<long long>> values;

    History(std::string inputString) : inputString(inputString) {
        std::vector<long long> rootValues;
        std::regex pattern(R"(-?\d+)");
        auto numbersBegin = std::sregex_iterator(inputString.begin(), inputString.end(), pattern);
        auto numbersEnd = std::sregex_iterator();
                
        for (std::sregex_iterator iter = numbersBegin; iter != numbersEnd; iter++) {
            std::smatch match = *iter;
            std::cout << match.str() << std::endl;
            rootValues.push_back(std::stoll(match.str()));
        }
        values.push_back(rootValues);
    }

    void ComputeDelta(std::vector<long long> values, int& delta) {
        if (IsAllZero(values)) {
            delta = 0;
            return;
        }

        std::vector<long long> diffValues;
        for (int i = 0; i < values.size() - 1; i++) {
            diffValues.push_back(values[i + 1] - values[i]); 
        }

        int delta = 0;
        ComputeDelta(diffValues, delta);
        long long nextValue = values[values.size() - 1] + delta;
        values.push_back(nextValue);
    }

private:
    bool IsAllZero(std::vector<long long> values) {
        for (auto& value : values) {
            if (value != 0)
                return false;
        }
        return true;
    }
};

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

    return 0;
}
