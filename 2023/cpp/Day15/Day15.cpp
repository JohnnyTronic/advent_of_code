#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

long hash(std::string& input) {
    long output = 0;
    for (char c : input) {
        output += int(c);
        output *= 17;
        output = output % 256;
    }
    return output;
}

class Lens {
public:
    std::string label;
    int focalLength;

    Lens(std::string label, int focalLength) : label(label), focalLength(focalLength) {}
};

int main()
{
    std::cout << "Advent of Code - Day 15!\n";

    std::ifstream ifs("input.txt");
    std::string parsedLine;  
    std::getline(ifs, parsedLine);

    long long score{};
    std::stringstream ss(parsedLine);
    std::string instruction;
    std::vector<std::string> instructions;
    while (std::getline(ss, instruction, ',')) {
        instructions.push_back(instruction);
        long output = hash(instruction);
        score += output;
    }
  
    std::cout << "PART 1 ANSWER - Sum of each instruction hash: " << std::to_string(score) << "\n";

    // Part 2
    std::vector<std::vector<Lens>> boxes;
    for (int i = 0; i < 256; i++) {
        std::vector<Lens> box;
        boxes.push_back(box);
    }

    std::regex instructionPattern(R"((\w+)([=-])(\d)?)");
    std::smatch match;
    for (auto instruction : instructions) {
        bool matchSuccess = std::regex_match(instruction, match, instructionPattern);
        std::string label = match[1];
        int labelHash = hash(label);
        std::string action = match[2];
        if (action == "=") {
            std::string focalLengthString = match[3];
            int focalLength = focalLengthString[0] - 48;

            // Check if same label already present
            std::vector<Lens>& box = boxes[labelHash];
            bool lensAlreadyPresent = false;
            for (auto iterator = box.begin(); iterator != box.end(); iterator++) {
                Lens& lens = *iterator;
                if (lens.label == label) {
                    lens.focalLength = focalLength;
                    lensAlreadyPresent = true;
                    break;

                }
            }
            if(!lensAlreadyPresent)
                boxes[labelHash].push_back(Lens(label, focalLength));
        }
        else if (action == "-") {
            std::vector<Lens>& box = boxes[labelHash];
            for (auto iterator = box.begin(); iterator != box.end(); iterator++) {
                auto lens = *iterator;
                if (lens.label == label) {
                    box.erase(iterator);
                    break;
                }
            }
        }
        else
            throw "Unrecognized action: " + action;        
    }

    long long totalFocusingPower = 0;
    for (int b = 0; b < boxes.size(); b++) {
        auto& box = boxes[b];
        for (int l = 0; l < box.size(); l++) {
            auto& lens = box[l];
            long focusingPower = (1 + b) * (l + 1) * lens.focalLength;
            totalFocusingPower += focusingPower;
        }
    }

    std::cout << "PART 2 ANSWER - Total focusing power of final lens arrangement: " << std::to_string(totalFocusingPower) << "\n";

    return 0;
}