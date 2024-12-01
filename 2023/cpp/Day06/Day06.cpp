#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <regex>
#include <map>

class RaceRecord {
public:
    int duration{};
    int distanceRecord{};

    RaceRecord(int duration, int distanceRecord) : duration(duration), distanceRecord(distanceRecord) {}
};

long long calcDistanceGivenHoldTime(long long holdTime, long long raceDuration) {
    return (raceDuration - holdTime) * holdTime;
}

int main()
{
    std::cout << "Advent of Core 2023 - Day 06!\n";

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    // Parse race durations
    std::string raceTimesString;
    std::getline(ifs, raceTimesString);
    std::regex raceTimesPattern(R"((\d+))");
    auto timesBegin = std::sregex_iterator(raceTimesString.begin(), raceTimesString.end(), raceTimesPattern);
    auto timesEnd = std::sregex_iterator();
    std::vector<int> raceTimes;
    for (auto iterator = timesBegin; iterator != timesEnd; iterator++) {
        std::smatch match = *iterator;
        std::string matchStr = match.str();
        raceTimes.push_back(std::stoi(matchStr));
    }
  
    std::string raceDistancesString;
    std::getline(ifs, raceDistancesString);
    std::regex raceDistancesPattern(R"((\d+))");
    auto distancesBegin = std::sregex_iterator(raceDistancesString.begin(), raceDistancesString.end(), raceDistancesPattern);
    auto distancesEnd = std::sregex_iterator();
    std::vector<int> raceDistances;
    for (auto iterator = distancesBegin; iterator != distancesEnd; iterator++) {
        std::smatch match = *iterator;
        std::string matchStr = match.str();
        raceDistances.push_back(std::stoi(matchStr));
    }

    std::vector<RaceRecord> raceRecords;
    for (int i = 0; i < raceTimes.size(); i++)
        raceRecords.push_back(RaceRecord(raceTimes[i], raceDistances[i]));

    std::vector<std::vector<RaceRecord>> simulations;
    for (auto raceRecord : raceRecords) {
        std::cout << "Simulating race - duration: " << raceRecord.duration << ", distance record: " << raceRecord.distanceRecord << std::endl;
        std::vector<RaceRecord> simulatedRaceRecords;
        for (int i = 0; i <= raceRecord.duration; i++) {
            int buttonHoldDuration = i;
            int distanceTravelled = calcDistanceGivenHoldTime(buttonHoldDuration, raceRecord.duration);
            std::cout << "Hold " << buttonHoldDuration << ", Distance " << distanceTravelled << std::endl;
            if (distanceTravelled > raceRecord.distanceRecord) {
                simulatedRaceRecords.push_back(RaceRecord(buttonHoldDuration, distanceTravelled));
            }
        }
        simulations.push_back(simulatedRaceRecords);
    }

    int winningStrategiesProduct = 1;
    for (auto& simulation : simulations) {
        winningStrategiesProduct *= simulation.size();
    }

    std::cout << "PART 1 ANSWER - Total number of winning strategies: " << winningStrategiesProduct << std::endl;
    
    // Part 2 - Bad kerning = one long race
    std::string combinedRaceDuration("");
    for (auto& duration : raceTimes) {
        combinedRaceDuration += std::to_string(duration);
    }
    long long singleRaceDuration(std::stoll(combinedRaceDuration));

    std::string combinedRaceRecord("");
    for (auto& distance : raceDistances) {
        combinedRaceRecord += std::to_string(distance);
    }
    long long singleRaceRecord(std::stoll(combinedRaceRecord));
    std::vector<RaceRecord> simulatedRaceRecords;
    for (int i = 0; i <= singleRaceDuration; i++) {
        long long buttonHoldDuration = i;
        long long distanceTravelled = calcDistanceGivenHoldTime(buttonHoldDuration, singleRaceDuration);
        //std::cout << "Hold " << buttonHoldDuration << ", Distance " << distanceTravelled << std::endl;
        if (distanceTravelled > singleRaceRecord) {
            simulatedRaceRecords.push_back(RaceRecord(buttonHoldDuration, distanceTravelled));
        }
    }

    std::cout << "PART 2 ANSWER - Total winning strategies for single long race: " << simulatedRaceRecords.size() << std::endl;

    return 0;
}