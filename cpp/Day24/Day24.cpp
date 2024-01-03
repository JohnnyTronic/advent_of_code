#include "Vec3.h"
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

        double pX = std::stoll(match[1].str());
        double pY = std::stoll(match[2].str());
        double pZ = std::stoll(match[3].str());
        double vX = std::stoll(match[4].str());
        double vY = std::stoll(match[5].str());
        double vZ = std::stoll(match[6].str());
      
        Hailstone* hailstone = new Hailstone(Vec3(pX, pY, pZ), Vec3(vX, vY, vZ));
        hailstones.push_back(hailstone);
    }

    return hailstones;
}

double DotProduct(const Vec3& lhs, const Vec3& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}


enum IntersectionResult {
    NEVER,    
    BEFORE,
    INSIDEAREA,
    OUTSIDEAREA
};

IntersectionResult TestForLineIntersection2D(Hailstone* A, Hailstone* B, double boundsMin, double boundsMax) {
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

    if (Px >= boundsMin && Px <= boundsMax && Py >= boundsMin && Py <= boundsMax) {
        Vec3 P(Px, Py, 0);
        Vec3 APos2D(A->position.x, A->position.y, 0);
        Vec3 AVel2D(A->velocity.x, A->velocity.y, 0);
        Vec3 AtoP = (P - APos2D).AsNormal();        
        Vec3 AVelUnit = AVel2D.AsNormal();
        double PdotA = DotProduct(AtoP, AVelUnit);

        Vec3 BPos2D(B->position.x, B->position.y, 0);
        Vec3 BVel2D(B->velocity.x, B->velocity.y, 0);
        Vec3 BtoP = (P - BPos2D).AsNormal();
        Vec3 BVelUnit = BVel2D.AsNormal();
        double PdotB = DotProduct(BtoP, BVelUnit);
        if (PdotA > 0 && PdotB > 0)
            return INSIDEAREA;
        else
            return BEFORE;          
    }

    return OUTSIDEAREA;
}

IntersectionResult TestForRayIntersection2D(Hailstone* A, Hailstone* B, double boundsMin, double boundsMax) {
    double dx = B->position.x - A->position.x;
    double dy = B->position.y - A->position.y;
    double det = B->velocity.x * A->velocity.y - B->velocity.y * A->velocity.x;
    if (abs(det) < std::numeric_limits<double>::epsilon())
        return NEVER;
    double u = (dy * B->velocity.x - dx * B->velocity.y) / det;
    double v = (dy * A->velocity.x - dx * A->velocity.y) / det;

    double pX = A->position.x + A->velocity.x * u;
    double pY = A->position.y + A->velocity.y * u;
    double pXB = B->position.x + B->velocity.x * v;
    double pYB = B->position.y + B->velocity.y * v;

    if (pX >= boundsMin && pX <= boundsMax && pY >= boundsMin && pY <= boundsMax) {
        if (u < 0 || v < 0)
            return BEFORE;
        return INSIDEAREA;
    }

    return OUTSIDEAREA;
}

void DoPart1(const std::vector<Hailstone*>& hailstones, double boundsMin, double boundsMax) {
    long testAreaIntersections = 0;  

    for (int i = 0; i < hailstones.size() - 1; i++) {
        for (int j = i + 1; j < hailstones.size(); j++) {
            Hailstone* stoneA = hailstones[i];
            Hailstone* stoneB = hailstones[j];

            IntersectionResult intersectionResult = TestForLineIntersection2D(stoneA, stoneB, boundsMin, boundsMax);
            IntersectionResult rayIntersectionResult = TestForRayIntersection2D(stoneA, stoneB, boundsMin, boundsMax);
            if (rayIntersectionResult == INSIDEAREA)
                testAreaIntersections++;

            // Inside
            // Inside
            // Outside
            // PastA
            // Never
            // Outside
            // PastBoth
            // Outside
            // PastB
            // PastBoth
        }
    }

    std::cout << "PART 1 ANSWER - How many intersections within the test area?: " << testAreaIntersections << "\n";
    //Incorrect Answers: 29143 (too high)
}

void DoPart2(const std::vector<Hailstone*>& hailstones) {
    // Not yet implemented
}

int main()
{
    std::cout << "Advent of Code - Day 24!\n";

    std::string fileName = "input.txt";
    double boundsMin = 200000000000000;
    double boundsMax = 400000000000000;
    if (false) {
        fileName = "test_input.txt";
        boundsMin = 7;
        boundsMax = 27;
    }
  
    auto hailstones = ParseInput(fileName);

    DoPart1(hailstones, boundsMin, boundsMax);
    DoPart2(hailstones);

}
