#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Sample {
public:
    int red = 0;
    int green = 0;
    int blue = 0;
};

class Game {
public:
    int id;
    std::vector<Sample> samples;

    Game(int id) : id(id) {}

    bool isPossibleGivenLimits(int redLimit, int greenLimit, int blueLimit) {
        for (auto sample : samples) {
            if (sample.red > redLimit)
                return false;

            if (sample.green > greenLimit)
                return false;

            if (sample.blue > blueLimit)
                return false;
        }
    }

    int calculateMinimumPower() {
        int minRed = 0;
        int minGreen = 0;
        int minBlue = 0;
        for (auto sample : samples) {
            minRed = std::max(minRed, sample.red);
            minGreen = std::max(minGreen, sample.green);
            minBlue = std::max(minBlue, sample.blue);
        }

        return minRed * minGreen * minBlue;
    }
};



int main()
{
    std::cout << "AoC 2023 Day 2 Part 1!\n";

    // Constraints
    int maxRed = 12;
    int maxGreen = 13;
    int maxBlue = 14;
    int targetSum = 8;
    std::vector<Game> games;

    std::ifstream ifs("input.txt");

    std::string line;
    int sum;
    while (ifs.good()) {
        std::getline(ifs, line);
        int colonPos = line.find(':');
        int prefixSize = 5; // Char count in "Game "
        std::string gameIdString = line.substr(5, colonPos - 5);
        int gameId = std::stoi(gameIdString);
    
        Game newGame(gameId);

        line.erase(0, colonPos + 2);
        while (line.length() > 0) {
            int semicolonPos = line.find(';');
            std::string sampleString = line.substr(0, semicolonPos);
            Sample newSample;

            while (sampleString.length() > 0) {
                int firstNonSpace = sampleString.find_first_not_of(' ');
                int spaceAfterNumber = sampleString.find_first_of(' ', firstNonSpace + 1);
            
                std::string countString = sampleString.substr(firstNonSpace, spaceAfterNumber - firstNonSpace);
                int colorCount = std::stoi(countString);
                sampleString.erase(0, spaceAfterNumber + 1);

                int commonPos = sampleString.find(',');
                std::string color = sampleString.substr(0, commonPos);
                if (color == "red")
                    newSample.red = colorCount;
                else if (color == "green")
                    newSample.green = colorCount;
                else if (color == "blue")
                    newSample.blue = colorCount;
                else
                    std::cout << "UNEXPECTED COLOR STRING: color";
                
                sampleString.erase(0, color.length() + 1);
            }

            newGame.samples.push_back(newSample);

            if (semicolonPos == std::string::npos)
                break;

            line.erase(0, semicolonPos + 2);
        }

        games.push_back(newGame);       
    }

    int score = 0;
    for (auto game : games) {
        std::cout << "GameId: " << game.id << "\n";
        if (game.isPossibleGivenLimits(maxRed, maxGreen, maxBlue))
            score += game.id;
    }

    std::cout << "Part 1 final score: " << score << "\n";

    // Part 2
    int sumPower = 0;
    for (auto game : games) {
        int minPower = game.calculateMinimumPower();
        std::cout << "GameId: " << game.id << ", minPower: " << minPower << "\n";
        sumPower += minPower;
    }

    std::cout << "Part 2 sum of minimum powers: " << sumPower << "\n";
    
    return 0;
}