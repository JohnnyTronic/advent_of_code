#include <iostream>
#include <fstream>
#include <sstream>

int main()
{
    std::cout << "AoC 2023 Day 2 Part 1!\n";

    // Constraints
    int maxRed = 12;
    int maxGreen = 13;
    int maxBlue = 14;
    int targetSum = 8;

    std::ifstream ifs("test_input.txt");

    std::string line;
    std::getline(ifs, line);
    int sum;
    while (ifs.good()) {
        int colonPos = line.find(':');
        std::cout << "colonPos: " << colonPos << std::endl;


        std::stringstream lineStream(line);
        std::string gameLabel;
        std::getline(lineStream, gameLabel, ':');

        std::string gameId;
        std::stringstream gameLabelStream(gameLabel);
        std::getline(gameLabelStream, gameId, ' '); // "Game"
        std::getline(gameLabelStream, gameId, ' '); // "##"
                
        std::string allGameResults;
        std::getline(lineStream, allGameResults, ':');

        std::stringstream allGameResultsStream(allGameResults);
        std::string gameResult;
        std::getline(allGameResultsStream, gameResult, ';');
        while (allGameResultsStream.good()) {
            std::stringstream gameResultStream(gameResult);
            std::cout << "gameResult: " << gameResult << std::endl;

            std::getline(allGameResultsStream, gameResult, ';');
        }

        

        // Read next line
        std::getline(ifs, line);
    }
    
    return 0;
}