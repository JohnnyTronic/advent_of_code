#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h> 


// test case:
//In the above example, card 1 has five winning numbers (41, 48, 83, 86, and 17) 
//	and eight numbers you have (83, 86, 6, 31, 17, 9, 48, and 53). 
//	Of the numbers you have, four of them (48, 83, 17, and 86) are winning numbers! 
//	That means card 1 is worth 8 points 
//	(1 for the first match, then doubled three times for each of the three matches after the first).

//Card 2 has two winning numbers(32 and 61), so it is worth 2 points.
//	Card 3 has two winning numbers(1 and 21), so it is worth 2 points.
//	Card 4 has one winning number(84), so it is worth 1 point.
//	Card 5 has no winning numbers, so it is worth no points.
//	Card 6 has no winning numbers, so it is worth no points.
//	So, in this example, the Elf's pile of scratchcards is worth 13 points.

class Card {
public:
	std::vector<int> winningNumbers, actualNumbers;
	int copies = 1;

	int GetActualWins() {
		int actualWins = 0;
		for (int i = 0; i < actualNumbers.size(); i++) {
			int actualNum = actualNumbers[i];
			if (std::find(winningNumbers.begin(), winningNumbers.end(), actualNum) != winningNumbers.end()) {
				actualWins++;
			}
		}
		return actualWins;
	}

	int GetPointValue() {
		int points = pow(2, GetActualWins() - 1);
		return points;
	}
};

int main() {

	std::ifstream ifs("input.txt", std::ifstream::in);
	std::string s;

	std::vector<Card> cards;


	while (ifs.good()) {
		std::getline(ifs, s);

		int colonPos = s.find(':');
		s.erase(0, colonPos + 2);
		//std::cout << "erased s: " << s << '\n';

		int pipePos = s.find('|');
		std::string winningString = s.substr(0, pipePos);
		//std::cout << "winnning string: " << winningString << '\n';
		s.erase(0, pipePos + 2);

		//std::cout << "erased s: " << s << '\n';

		Card card;

		while (s.length() > 0) {
			int num = std::stoi(s.substr(0, 3));
			card.actualNumbers.push_back(num);
			s.erase(0, 3);
		}

		while (winningString.length() > 0) {
			int num = std::stoi(winningString.substr(0, 3));
			card.winningNumbers.push_back(num);
			winningString.erase(0, 3);
		}

		cards.push_back(card);
	}

	//int sum = 0;
	//for (auto& card : cards) {
	//	std::cout <<"card wins: "<<card.GetActualWins()<<", point value : " << card.GetPointValue() << '\n';
	//	sum += card.GetPointValue();
	//}
	//std::cout << "sum: " << sum << '\n';

	for (int activeCardIndex = 0; activeCardIndex < cards.size(); activeCardIndex++) {
		Card activeCard = cards[activeCardIndex];
		int wins = activeCard.GetActualWins();
		for (int effectedCardIndex = activeCardIndex + 1; effectedCardIndex <= activeCardIndex + wins && effectedCardIndex < cards.size(); effectedCardIndex++) {
			cards[effectedCardIndex].copies += activeCard.copies;
		}
	}

	int cardSum = 0;
	for (auto& card : cards) {
		cardSum += card.copies;
	}

	std::cout << "card sum: " << cardSum;

	return 0;
}