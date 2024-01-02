#include "Vec2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

struct Hailstone {
    Vec3 position;
    Vec3 velocity;

    Hailstone(Vec3 position, Vec3 velocity) : position(position), velocity(velocity) {

    };
};

std::vector<Hailstone*> ParseInput(const std::string& fileName) {
    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedLines;

    std::regex coordPattern(R"((-?\d+),\s+(-?\d+),\s+(-?\d+)\s+@\s+(-?\d+),\s+(-?\d+),\s+(-?\d+))");

    std::vector<Hailstone*> hailstones;
    while (ifs.good()) {
        std::getline(ifs, parsedLine);
        std::smatch match;
        auto success = std::regex_search(parsedLine, match, coordPattern);
        if (!success)
            throw "Regex failure";

        long long pX = std::stoll(match[1].str());
        long long pY = std::stoll(match[2].str());
        long long pZ = std::stoll(match[3].str());
        long long vX = std::stoll(match[4].str());
        long long vY = std::stoll(match[5].str());
        long long vZ = std::stoll(match[6].str());
      
        Hailstone* hailstone = new Hailstone(Vec3(pX, pY, pZ), Vec3(vX, vY, vZ));
        hailstones.push_back(hailstone);
    }

    return hailstones;
}

enum IntersectionResult {
    NEVER,    
    BEFORE,
    INSIDEAREA,
    OUTSIDEAREA
};

IntersectionResult TestForIntersection2D(Hailstone* A, Hailstone* B, long long boundsMin, long long boundsMax) {
    // Based on https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection

    Vec3 A2 = (A->position + A->velocity);
    Vec3 B2 = (B->position + B->velocity);

    double x1 = A->position.x;
    double y1 = A->position.y;
    double x2 = A2.x;
    double y2 = A2.y;
    double x3 = B->position.x;
    double y3 = B->position.y;
    double x4 = B2.x;
    double y4 = B2.y;

    double PxNumerator = (((x1 * y2) - (y1 * x2)) * (x3 - x4)) - ((x1 - x2) * ((x3 * y4) - (y3 * x4)));
    double PxDenominator = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));
    double PyNumerator = ((x1 * y2 - y1 * x2) * (y3 - y4)) - ((y1 - y2) * (x3 * y4 - y3 * x4));
    double PyDenominator = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

    if (PxDenominator == 0 || PyDenominator == 0)
        return NEVER;

    double Px = PxNumerator / PxDenominator;
    double Py = PyNumerator / PyDenominator;

    if(Px >= boundsMin && Px <= boundsMax && Py >= boundsMin && Py <= boundsMax)
        return INSIDEAREA;

    return OUTSIDEAREA;
}

void DoPart1(const std::vector<Hailstone*>& hailstones, long long boundsMin, long long boundsMax) {
    long testAreaIntersections = 0;  

    for (int i = 0; i < hailstones.size() - 1; i++) {
        for (int j = i + 1; j < hailstones.size(); j++) {
            Hailstone* stoneA = hailstones[i];
            Hailstone* stoneB = hailstones[j];

            IntersectionResult intersectionResult = TestForIntersection2D(stoneA, stoneB, boundsMin, boundsMax);
            if (intersectionResult == INSIDEAREA)
                testAreaIntersections++;
        }
    }

    std::cout << "PART 1 ANSWER - How many intersections within the test area?: " << testAreaIntersections << "\n";
}

void DoPart2(const std::vector<Hailstone*>& hailstones) {
    // Not yet implemented
}

int main()
{
    std::cout << "Advent of Code - Day 24!\n";

    std::string fileName = "input.txt";
    long long boundsMin = 200000000000000;
    long long boundsMax = 400000000000000;
    if (true) {
        fileName = "test_input.txt";
        boundsMin = 7;
        boundsMax = 27;
    }
  
    auto hailstones = ParseInput(fileName);

    DoPart1(hailstones, boundsMin, boundsMax);
    DoPart2(hailstones);

}
