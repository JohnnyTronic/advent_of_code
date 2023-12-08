#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <regex>
#include <array>

class Vec2 {
public:
    int x = 0;
    int y = 0;

    Vec2() {};
    Vec2(int x, int y) : x(x), y(y) {}

    friend Vec2 operator+(const Vec2 left, const Vec2 right) {
        return Vec2(left.x + right.x, left.y + right.y);
    }
};

template <typename T>
class Grid2D {
public:    
    
    std::vector<std::vector<T>> content;

    int getWidth() {
        return this->content.size() == 0 ? 0 : this->content[0].size() == 0 ? 0 : this->content[0].size();
    }

    int getHeight() {
        return this->content.size();
    }
    
    bool isOnGrid(Vec2& position) {
        return position.x >= 0 && position.x < this->getWidth() && position.y >= 0 && position.y < this->getHeight();
    }
    
    std::vector<Vec2> GenerateNeighbourPositions(const Vec2& position) {

        std::vector<Vec2> possibleNeighbourPositions;

        possibleNeighbourPositions.push_back(position + Vec2(-1, -1));
        possibleNeighbourPositions.push_back(position + Vec2(0, -1));
        possibleNeighbourPositions.push_back(position + Vec2(1, -1));
        possibleNeighbourPositions.push_back(position + Vec2(-1, 0));
        possibleNeighbourPositions.push_back(position + Vec2(1, 0));
        possibleNeighbourPositions.push_back(position + Vec2(-1, 1));
        possibleNeighbourPositions.push_back(position + Vec2(0, 1));
        possibleNeighbourPositions.push_back(position + Vec2(1, 1));

        std::vector<Vec2> onGridNeighbours;
        for (auto& possibleNeighbour : possibleNeighbourPositions) {
            if(this.isOnGrid(possibleNeighbour)) {
                onGridNeighbours.push_back(possibleNeighbour);
            }
        }

        return onGridNeighbours;
    }
};

class PartNumber {
public:
    int value;
    Vec2 gridPosition;
    int length;

    PartNumber(int value, Vec2 gridPosition, int length) : value(value), gridPosition(gridPosition), length(length) {}
};

class Symbol {
public:
    std::string value;
    Vec2 gridPosition;

    Symbol(std::string value, Vec2 gridPosition) : value(value), gridPosition(gridPosition) {}

    friend std::ostream& operator<< (std::ostream& out, const Symbol& symbol) {
        out << "Symbol[\'" << symbol.value << ", (" << symbol.gridPosition.x << "," << symbol.gridPosition.y << ")";
    }
};

int main()
{
    std::cout << "Advent of Code 2023 - Day 3\n";

    std::vector<PartNumber> partNumbers;
    int sumOfPartNumbers = 0;

    std::ifstream ifs("test_input.txt");
    //std::ifstream ifs("input.txt");
    std::string line;    

    Grid2D<char> inputGrid;
    Grid2D<bool> dirtyGrid;
    std::regex partNumberPattern("[0-9]+");
    std::regex symbolPattern("[^0-9.]+");
    int rowIndex = 0;
    while (std::getline(ifs, line)) {
        // Search for Part Numbers
        auto partNumberMatchesBegin = std::sregex_iterator(line.begin(), line.end(), partNumberPattern);
        std::sregex_iterator partNumberMatchesEnd;
        for (auto iter = partNumberMatchesBegin; iter != partNumberMatchesEnd; iter++) {
            std::smatch partNumberMatch = *iter;
            std::cout << "Found part number: " << partNumberMatch.str() << std::endl;
            auto partNumberString = partNumberMatch.str();
            auto partNumberValue = std::stoi(partNumberString);
            PartNumber newPartNumber(partNumberValue, Vec2(partNumberMatch.position(), rowIndex), partNumberString.length());
            partNumbers.push_back(newPartNumber);
        }

        // Search for Symbols
        auto symbolMatchesBegin = std::sregex_iterator(line.begin(), line.end(), symbolPattern);
        std::sregex_iterator symbolMatchesEnd;
        for (auto iter = symbolMatchesBegin; iter != symbolMatchesEnd; iter++) {
            std::smatch symbolMatch = *iter;
            std::cout << "Found symbol: " << symbolMatch.str() << std::endl;
            auto symbolString = symbolMatch.str();
            Symbol newSymbol(symbolString, Vec2(symbolMatch.position(), rowIndex));
        }

        std::vector<char> newRow;
        std::vector<bool> dirtyRow;
        for (auto c : line) {
            newRow.push_back(c);
            dirtyRow.push_back(false);
        }
        inputGrid.content.push_back(newRow);
        dirtyGrid.content.push_back(dirtyRow);

        rowIndex++;
    }
    std::cout << "Grid Dimensions: " << inputGrid.getWidth() << "w x " << inputGrid.getHeight() << "h" << std::endl;
        
    /*std::set<int> visitedCells;
    auto matchesBegin = std::sregex_iterator(fileContent.begin(), fileContent.end(), nonSymbolCharacters);
    auto matchesEnd = std::sregex_iterator();
    std::cout << "Found " << std::distance(matchesBegin, matchesEnd) << " symbols.\n";
    for (auto iter = matchesBegin; iter != matchesEnd; iter++) {
        std::smatch match = *iter;
        std::cout << "Targetting symbol: " << match.str() << " at location " << match.position() << std::endl;
    }*/

    
    std::cout << "Answer for Part 1 - Sum of part numbers: " << sumOfPartNumbers << std::endl;
}