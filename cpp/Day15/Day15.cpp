#include <iostream>
#include <fstream>
#include <sstream>

long hash(std::string& input) {
    long output = 0;
    for (char c : input) {
        output += int(c);
        output *= 17;
        output = output % 256;
    }
    return output;
}

int main()
{
    std::cout << "Advent of Code - Day 15!\n";

    std::ifstream ifs("input.txt");
    std::string parsedLine;  
    std::getline(ifs, parsedLine);

    long long score{};
    std::stringstream ss(parsedLine);
    std::string instruction;
    while (std::getline(ss, instruction, ',')) {
        long output = hash(instruction);
        score += output;
    }
  
    std::cout << "PART 1 ANSWER - Sum of each instruction hash: " << std::to_string(score) << "\n";

    return 0;
}