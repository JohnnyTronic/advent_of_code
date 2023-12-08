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

class GridItem {
public:
    std::string str;
    int value;
    Vec2 gridPosition;
    int length;

    GridItem(int value, Vec2 gridPosition, int length) : value(value), gridPosition(gridPosition), length(length) {}
    GridItem(std::string str, Vec2 gridPosition) : str(str), value(0), gridPosition(gridPosition), length(1), isSymbol(true) {}

    int minBoundX() {
        return gridPosition.x;
    }

    int maxBoundX() {
        return gridPosition.x + length;
    }

    int minBoundY() {
        return gridPosition.y;
    }

    int maxBoundY() {
        return gridPosition.y + 1;
    }

    friend std::ostream& operator<< (std::ostream& out, const GridItem& gridItem) {
        std::string output;
        if (gridItem.isSymbol) {
            output = gridItem.str;
        }
        else
            output = std::to_string(gridItem.value);
        out << "Grid item [" << output << "], (" << gridItem.gridPosition.x << "," << gridItem.gridPosition.y << ") length: " << gridItem.length;
        return out;
    }

private:
    bool isSymbol = false;
    
};

int main()
{
    std::cout << "Advent of Code 2023 - Day 3\n";
        
    std::vector<GridItem> partNumbers;
    std::vector<GridItem> symbols;

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");
    std::string line;    
 
    std::regex partNumberPattern("[0-9]+");
    std::regex symbolPattern("[^0-9.]+");
    int rowIndex = 0;
    while (std::getline(ifs, line)) {
        // Search for Part Numbers
        auto partNumberMatchesBegin = std::sregex_iterator(line.begin(), line.end(), partNumberPattern);
        std::sregex_iterator partNumberMatchesEnd;
        for (auto iter = partNumberMatchesBegin; iter != partNumberMatchesEnd; iter++) {
            std::smatch partNumberMatch = *iter;
            auto partNumberString = partNumberMatch.str();
            auto partNumberValue = std::stoi(partNumberString);
            GridItem newPartNumber(partNumberValue, Vec2(partNumberMatch.position(), rowIndex), partNumberString.length());
            partNumbers.push_back(newPartNumber);
            std::cout << "Found part number: " << newPartNumber << std::endl;
        }

        // Search for Symbols
        auto symbolMatchesBegin = std::sregex_iterator(line.begin(), line.end(), symbolPattern);
        std::sregex_iterator symbolMatchesEnd;
        for (auto iter = symbolMatchesBegin; iter != symbolMatchesEnd; iter++) {
            std::smatch symbolMatch = *iter;
            auto symbolString = symbolMatch.str();
            GridItem newSymbol(symbolString, Vec2(symbolMatch.position(), rowIndex));
            symbols.push_back(newSymbol);
            std::cout << "Found symbol: " << newSymbol << std::endl;
        }

        rowIndex++;
    }
    std::cout << "Part Number Counts: " << partNumbers.size() << ", Symbols Count: " << symbols.size() << std::endl;
      
    // Collision Detection
    std::vector<GridItem> collidingParts;
    for (auto& partNumber : partNumbers) {
        //std::cout << "Checking " << partNumber.value;
        for (auto& symbol : symbols) {
            //std::cout << " against " << symbol.str << std::endl;
            if (partNumber.minBoundX() > symbol.maxBoundX())
                continue;
            if (partNumber.maxBoundX() < symbol.minBoundX())
                continue;
            if (partNumber.minBoundY() > symbol.maxBoundY())
                continue;
            if (partNumber.maxBoundY() < symbol.minBoundY())
                continue;

            collidingParts.push_back(partNumber);
            std::cout << "Collision between " << partNumber.value << " & " << symbol.str << std::endl;
        }
    }
    std::cout << "Colliding parts count: " << collidingParts.size() << std::endl;

    int sumOfCollidingPartNumbers = 0;
    for (auto& collidingPart : collidingParts) {
        sumOfCollidingPartNumbers += collidingPart.value;
    }

    
    std::cout << "Answer for Part 1 - Sum of symbol-adjacent part numbers: " << sumOfCollidingPartNumbers << std::endl;
}