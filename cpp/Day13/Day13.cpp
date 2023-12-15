#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Pattern {
public:
    std::vector<std::string> content;

    int GetWidth() {
        return content[0].size();
    }

    int GetHeight() {
        return content.size();
    }
};

long long CalculatePatternScore(Pattern& pattern) {
    // Check for vertical symmetry
    for (int axisAfterRow = 0; axisAfterRow < pattern.GetHeight() - 1; axisAfterRow++) {
        for (int column = 0; column < pattern.GetWidth(); column++) {
            for (int focusRow = 0; focusRow <= axisAfterRow; focusRow++) {
                float distanceToMirrorAxis = (axisAfterRow + 0.5) - focusRow;
                int testRow = focusRow + (distanceToMirrorAxis * 2);
                if (testRow >= pattern.GetHeight())
                    continue; // We're off the far end of the pattern. Nothing to check but it doesn't mean the symmetry axis is wrong

                char focusChar = pattern.content[focusRow][column];
                char testChar = pattern.content[testRow][column];
                if (focusChar != testChar) {
                    // No symmetry here. Advance the mirror axis
                    column = std::numeric_limits<int>::max() - 1;
                    break;
                }
            }

            if (column == pattern.GetWidth() - 1) {
                return (axisAfterRow + 1) * 100;
            }
        }

    }

    // Check for horizontal symmerty
    // "Focus___" is the item we're comparing from
    // "Test___" is the item we're comparing Focus against
    for (int axisAfterColumn = 0; axisAfterColumn < pattern.GetWidth() - 1; axisAfterColumn++) {
        for (int row = 0; row < pattern.GetHeight(); row++) {
            auto& focusRow = pattern.content[row];

            for (int focusColumn = 0; focusColumn <= axisAfterColumn; focusColumn++) {
                float distanceToMirrorAxis = (axisAfterColumn + 0.5) - focusColumn;
                int testColumn = focusColumn + (distanceToMirrorAxis * 2);
                if (testColumn >= pattern.GetWidth())
                    continue; // We're off the far end of the pattern, nothing to check but it doesn't mean the symmetry axis is wrong

                char focusChar = focusRow[focusColumn];
                char testChar = focusRow[testColumn];
                if (focusChar != testChar) {
                    // No symmetry here, advance the mirror axis
                    row = std::numeric_limits<int>::max() -1 ;
                    break;                    
                }
            }

            if (row == pattern.GetHeight() - 1) {
                return axisAfterColumn + 1;
            }
        }
        

    }

    return 0;
}

int main()
{
    std::cout << "Advent of Code - Day 13!\n";

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    std::vector<Pattern> patterns;
    Pattern newPattern{};
    std::string parsedLine;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        if (parsedLine.empty()) {
            patterns.push_back(newPattern);
            newPattern = Pattern();
            continue;
        }

        newPattern.content.push_back(parsedLine);
    }
    patterns.push_back(newPattern);

    long long score = 0;
    for (auto& pattern : patterns) {
        long long patternScore = CalculatePatternScore(pattern);
        score += patternScore;
    }

    std::cout << "PART 1 ANSWER - Total scored lines: " << score << "\n";

    return 0;
}
