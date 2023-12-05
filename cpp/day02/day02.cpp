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
        

        //std::stringstream lineStream(line);
        //std::string gameLabel;
        //std::getline(lineStream, gameLabel, ':');

        //std::string gameId;
        //std::stringstream gameLabelStream(gameLabel);
        //std::getline(gameLabelStream, gameId, ' '); // "Game"
        //std::getline(gameLabelStream, gameId, ' '); // "##"
        //        
        //std::string allGameResults;
        //std::getline(lineStream, allGameResults, ':');

        //std::stringstream allGameResultsStream(allGameResults);
        //std::string gameResult;
        //std::getline(allGameResultsStream, gameResult, ';');
        //while (allGameResultsStream.good()) {
        //    std::stringstream gameResultStream(gameResult);
        //    std::cout << "gameResult: " << gameResult << std::endl;

        //    std::getline(allGameResultsStream, gameResult, ';');
        //}

        //

        //// Read next line
        //std::getline(ifs, line);
    }

    int score = 0;
    for (auto game : games) {
        std::cout << "GameId: " << game.id << "\n";
        if (game.isPossibleGivenLimits(maxRed, maxGreen, maxBlue))
            score += game.id;
    }

    std::cout << "Final score: " << score << "\n";
    
    return 0;
}