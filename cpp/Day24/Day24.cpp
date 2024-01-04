//#include "Vec3.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <regex>
//#include <Eigen/Core>
//#include <Eigen/Dense>
#include "Vec3.h"
#include <regex>

#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <map>
#include <queue>
#include <set>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <numeric>
#include <unordered_set>
#include <cassert>
#include <sstream>
#include <queue>
#include <bitset>
#include <Eigen/Core>
#include <Eigen/Dense>

struct Hailstone {
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
};

std::vector<Hailstone> ParseInput(const std::string& fileName) {
    std::ifstream ifs(fileName);
    std::string parsedLine;
    std::vector<std::string> parsedLines;

    std::regex coordPattern(R"((-?\d+),\s+(-?\d+),\s+(-?\d+)\s+@\s+(-?\d+),\s+(-?\d+),\s+(-?\d+))");

    std::vector<Hailstone> hailstones;
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
      
        Hailstone h;
        h.position[0] = pX;
        h.position[1] = pY;
        h.position[2] = pZ;
        h.velocity[0] = vX;
        h.velocity[1] = vY;
        h.velocity[2] = vZ;        
        hailstones.push_back(h);
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

IntersectionResult TestForLineIntersection2D(const Hailstone& A, const Hailstone& B, const double boundsMin, const double boundsMax) {
    // Based on https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection

    auto A2 = (A.position + A.velocity);
    auto B2 = (B.position + B.velocity);

    double x1 = A.position.x();
    double y1 = A.position.y();
    double x2 = A2.x();
    double y2 = A2.y();
    double x3 = B.position.x();
    double y3 = B.position.y();
    double x4 = B2.x();
    double y4 = B2.y();

    double PxNumerator = (((x1 * y2) - (y1 * x2)) * (x3 - x4)) - ((x1 - x2) * ((x3 * y4) - (y3 * x4)));
    double PxDenominator = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));
    double PyNumerator = ((x1 * y2 - y1 * x2) * (y3 - y4)) - ((y1 - y2) * (x3 * y4 - y3 * x4));
    double PyDenominator = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

    if (PxDenominator == 0 || PyDenominator == 0)
        return NEVER;

    double Px = PxNumerator / PxDenominator;
    double Py = PyNumerator / PyDenominator;

    if (Px >= boundsMin && Px <= boundsMax && Py >= boundsMin && Py <= boundsMax) {
        Eigen::Vector3d P(Px, Py, 0);        
        Eigen::Vector3d AtoP = (P - A.position).normalized();        
        Eigen::Vector3d AVelUnit = A.velocity.normalized();
        //double PdotA = DotProduct(AtoP, AVelUnit);
        double PdotA = AtoP.dot(AVelUnit);
        
        Eigen::Vector3d BtoP = (P - B.position).normalized();
        Eigen::Vector3d BVelUnit = B.velocity.normalized();
        double PdotB = BtoP.dot(BVelUnit);
        if (PdotA > 0 && PdotB > 0)
            return INSIDEAREA;
        else
            return BEFORE;          
    }

    return OUTSIDEAREA;
}

IntersectionResult TestForRayIntersection2D(const Hailstone& A, const Hailstone& B, const double boundsMin, const double boundsMax) {
    double dx = B.position.x() - A.position.x();
    double dy = B.position.y() - A.position.y();
    double det = B.velocity.x() * A.velocity.y() - B.velocity.y() * A.velocity.x();
    if (abs(det) < std::numeric_limits<double>::epsilon())
        return NEVER;
    double u = (dy * B.velocity.x() - dx * B.velocity.y()) / det;
    double v = (dy * A.velocity.x() - dx * A.velocity.y()) / det;

    double pX = A.position.x() + A.velocity.x() * u;
    double pY = A.position.y() + A.velocity.y() * u;
    double pXB = B.position.x() + B.velocity.x() * v;
    double pYB = B.position.y() + B.velocity.y() * v;

    if (pX >= boundsMin && pX <= boundsMax && pY >= boundsMin && pY <= boundsMax) {
        if (u < 0 || v < 0)
            return BEFORE;
        return INSIDEAREA;
    }

    return OUTSIDEAREA;
}

void DoPart1(const std::vector<Hailstone>& hailstones, double boundsMin, double boundsMax) {
    long testAreaIntersections = 0;  

    for (int i = 0; i < hailstones.size() - 1; i++) {
        for (int j = i + 1; j < hailstones.size(); j++) {
            const Hailstone& stoneA = hailstones[i];
            const Hailstone& stoneB = hailstones[j];

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

Eigen::Matrix3d crossMatrix(Eigen::Vector3d v) {
    Eigen::Matrix3d result;
    result << 0, -v[2], v[1],
        v[2], 0, -v[0],
        -v[1], v[0], 0;
    return result;
}

void DoPart2(const std::vector<Hailstone>& hailstones) {
  
    Eigen::MatrixXd M(6, 6);
    Eigen::VectorXd rhs(6);
    rhs.segment<3>(0) = -hailstones[0].position.cross(hailstones[0].velocity) + hailstones[1].position.cross(hailstones[1].velocity);
    rhs.segment<3>(3) = -hailstones[0].position.cross(hailstones[0].velocity) + hailstones[2].position.cross(hailstones[2].velocity);
    M.block<3, 3>(0, 0) = crossMatrix(hailstones[0].velocity) - crossMatrix(hailstones[1].velocity);
    M.block<3, 3>(3, 0) = crossMatrix(hailstones[0].velocity) - crossMatrix(hailstones[2].velocity);
    M.block<3, 3>(0, 3) = -crossMatrix(hailstones[0].position) + crossMatrix(hailstones[1].position);
    M.block<3, 3>(3, 3) = -crossMatrix(hailstones[0].position) + crossMatrix(hailstones[2].position);
    Eigen::VectorXd result = M.inverse() * rhs;
    std::cout << "pos: " << result.segment<3>(0) << " vel: " << result.segment<3>(3) << std::endl;
    int64_t sum = 0;
    for (int i = 0; i < 3; i++)
        sum += int64_t(result[i]);
    std::cout << "Sum of position coefficients: " << sum << std::endl;

    // Incorrect answers: 47, 848947587263016
}

Eigen::Matrix3d getCoeffMat(const Eigen::Vector3d& v) {
    Eigen::Matrix3d coeffMat;
    coeffMat << 0, -v[2], v[1], v[2], 0, -v[0], -v[1], v[0], 0;
    return coeffMat;
}

//// Based on https://github.com/TSoli/advent-of-code/blob/main/2023/day24b/solution.cpp
void DoPart2Alternate(const std::vector<Hailstone>& hailstones) {

    // Numerical stability issues requires some consensurs building
    std::unordered_map<int64_t, int> freqs;
    std::unordered_map<int64_t, Eigen::VectorXd> results;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, hailstones.size() - 1);
    for (int i = 0; i < 100; i++) {
        int p0{ distr(gen) };
        int p1{ distr(gen) };
        int p2{ distr(gen) };
        int p3{ distr(gen) };

        if (p0 == p1 || p0 == p2 || p0 == p3 || p1 == p2 || p1 == p3 || p2 == p3) {
            continue;
        }


        Eigen::MatrixXd equations(6, 6);
        Eigen::VectorXd rhs(6);

        rhs.segment<3>(0) = hailstones[0].position.cross(hailstones[0].velocity) - hailstones[1].position.cross(hailstones[1].velocity);
        rhs.segment<3>(3) = hailstones[2].position.cross(hailstones[2].velocity) - hailstones[3].position.cross(hailstones[3].velocity);
        equations.block<3, 3>(0, 0) = getCoeffMat(hailstones[1].velocity - hailstones[0].velocity);
        equations.block<3, 3>(3, 0) = getCoeffMat(hailstones[3].velocity - hailstones[2].velocity);
        equations.block<3, 3>(0, 3) = getCoeffMat(hailstones[0].position - hailstones[1].position);
        equations.block<3, 3>(3, 3) = getCoeffMat(hailstones[2].position - hailstones[3].position);
        Eigen::VectorXd result = equations.inverse() * rhs;

        int64_t sum{ 0 };
        for (int i = 0; i < 3; ++i) {
            sum += std::roundl(result[i]);
        }
        freqs[sum]++;
        results[sum] = result;
        std::cout << "Result: " << result[0] << "," << result[1] << "," << result[2] << "--" << result[3] << "," << result[4] << "," << result[5] << "\n";
    }

    int maxFreq{ 0 };
    int64_t sum{ 0 };
    for (const auto& freq : freqs) {
        auto k = freq.first;
        auto v = freq.second;
        if (v > maxFreq) {
            maxFreq = v;
            sum = k;
        }
    }

    std::cout << "Sum of position coefficients: " << sum << " " << maxFreq << "\n";

    // Incorrect answers: -2147483648, 56568377, (Without rounding) 848947587263031   
    // With just round(): 848947587263033   
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

    //DoPart1(hailstones, boundsMin, boundsMax);
    //DoPart2(hailstones);
    DoPart2Alternate(hailstones);
}
