// 2024_AoC1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

int main()
{
    std::map<std::string, std::string> writtenNumbers;
    writtenNumbers["one"] = '1';
    writtenNumbers["two"] = '2';
    writtenNumbers["three"] = '3';
    writtenNumbers["four"] = '4';
    writtenNumbers["five"] = '5';
    writtenNumbers["six"] = '6';
    writtenNumbers["seven"] = '7';
    writtenNumbers["eight"] = '8';
    writtenNumbers["nine"] = '9';

    //for (std::map<std::string, std::string>::iterator iter = writtenNumbers.begin(); iter != writtenNumbers.end(); iter++)
    //{
    //    std::cout << "key: " << iter->first << ", value: " << iter->second << '\n';
    //    std::cout << "manual access: " << writtenNumbers["seven"] << '\n';
    //}

    //return 0;

    std::ifstream ifs("input.txt", std::ifstream::in);

    std::string s;
    int sum = 0;
    std::getline(ifs, s);
    while (ifs.good()) {
        int i = 0;
        char firstChar = s[i];
        bool isDigit = std::isdigit(firstChar);

        while (i < s.length() && !isDigit) {
            firstChar = s[i];
            isDigit = std::isdigit(firstChar);

            if (isDigit) break;

            for (std::map<std::string, std::string>::iterator iter = writtenNumbers.begin(); iter != writtenNumbers.end(); iter++)
            {
                auto expectedString = iter->first;
                std::string substring = s.substr(i, expectedString.length());
                if (substring == expectedString)
                {
                    firstChar = iter->second[0];
                    isDigit = true;
                    break;
                }
            }

            i++;
        }

        int j = s.length() - 1;
        char lastChar = s[j];
        isDigit = std::isdigit(lastChar);

        while (j > 0 && !isDigit) {
            j--;
            lastChar = s[j];
            isDigit = std::isdigit(lastChar);
            if (isDigit) break;

            for (std::map<std::string, std::string>::iterator iter = writtenNumbers.begin(); iter != writtenNumbers.end(); iter++)
            {
                auto expectedString = iter->first;
                std::string substring = s.substr(j, expectedString.length());
                if (substring == expectedString)
                {
                    lastChar = iter->second[0];
                    isDigit = true;
                    break;
                }
            }
        }

        std::cout << "given " << s << ", found first digit " << firstChar << ", found last digit " << lastChar << "\n";
        char numberString[] = { firstChar, lastChar, '\0' };
        int number = atoi(numberString);
        sum += number;

        std::getline(ifs, s);
    }

    ifs.close();
    std::cout << "final sum: " << sum;
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
