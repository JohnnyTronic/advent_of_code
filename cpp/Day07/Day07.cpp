#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <regex>
#include <map>

using std::cout;

int GetCardIndex(char label) {
    switch (label) {
    case '2': return 0;
    case '3': return 1;
    case '4': return 2;
    case '5': return 3;
    case '6': return 4;
    case '7': return 5;
    case '8': return 6;
    case '9': return 7;
    case 'T': return 8;
    case 'J': return 9;
    case 'Q': return 10;
    case 'K': return 11;
    case 'A': return 12;       
    default: throw "Unrecognized card index: " + label;
    }
}

enum class HandType {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind,
};

static std::string GetHandTypeName(HandType& handType) {
    switch (handType) {
    case HandType::FiveOfAKind: return "FiveOfAKind";
    case HandType::FourOfAKind: return "FourOfAKind";
    case HandType::FullHouse: return "FullHouse";
    case HandType::ThreeOfAKind: return "ThreeOfAKind";
    case HandType::TwoPair: return "TwoPair";
    case HandType::OnePair: return "OnePair";
    default: return "HighCard";
    }
}

class Hand {
public:
    std::string cards;
    int bid;

    Hand(std::string cards, int bid) : cards(cards), bid(bid) {}
};

HandType ClassifyHand(std::string& cards) {
    // Sort a copy of the input first (simplifies the regex work to have similar cards grouped)
    std::string input = cards;
    std::sort(input.begin(), input.end());

    std::regex fiveOfAKindPattern(R"((\w)\1\1\1\1)");
    if (std::regex_search(input, fiveOfAKindPattern)) {
        return HandType::FiveOfAKind;
    }

    std::regex fourOfAKindPattern(R"((\w)\1\1\1)");
    if (std::regex_search(input, fourOfAKindPattern)) {
        return HandType::FourOfAKind;
    }

    std::regex fullHousePattern(R"((?:(\w)\1\1(\w)\2)|(?:(\w)\3(\w)\4\4))");
    if (std::regex_search(input, fullHousePattern)) {
        return HandType::FullHouse;
    }

    std::regex threeOfAKindPattern(R"(.*(\w)(?=.*\1.*\1))");
    if (std::regex_search(input, threeOfAKindPattern)) {
        return HandType::ThreeOfAKind;
    }

    std::regex twoPairPattern(R"(.*(\w)(?=.*\1).*(\w)(?=.*\2))");
    if (std::regex_search(input, twoPairPattern)) {
        return HandType::TwoPair;
    }

    std::regex onePairPattern(R"(.*(\w)(?=.*\1))");
    if (std::regex_search(input, onePairPattern)) {
        return HandType::OnePair;
    }

    return HandType::HighCard;
}

void TestClassify(std::string& cards, HandType expectedResult) {
    HandType result = ClassifyHand(cards);
    std::string matchesExpectedResult = expectedResult == result ? "CORRECT" : "WRONG";
    std::cout << "TestClassify: " << cards << " -> " << GetHandTypeName(result) << ", Matches expected result?: " <<  matchesExpectedResult << std::endl;
}

bool SortTwoHands(Hand& left, Hand& right) {
    HandType leftClass = ClassifyHand(left.cards);
    HandType rightClass = ClassifyHand(right.cards);
    int leftIndex = static_cast<int>(leftClass);
    int rightIndex = static_cast<int>(rightClass);

    if (leftIndex < rightIndex)
        return true;

    if (leftIndex > rightIndex)
        return false;

    if (leftIndex == rightIndex) {
        for (int i = 0; i < left.cards.length(); i++) {
            int leftCardIndex = GetCardIndex(left.cards[i]);
            int rightCardIndex = GetCardIndex(right.cards[i]);
            if (leftCardIndex < rightCardIndex)
                return true;

            if (leftCardIndex > rightCardIndex)
                return false;

            continue;
        }

        throw "Are these identical hands?";
    }
}

void TestClassifier() {
    std::string fiveOfAKindHand("KKKKK");
    TestClassify(fiveOfAKindHand, HandType::FiveOfAKind);

    std::string fourOfAKindHand("24222");
    TestClassify(fourOfAKindHand, HandType::FourOfAKind);

    std::string fullHouseHand("77J7J");
    TestClassify(fullHouseHand, HandType::FullHouse);

    std::string threeOfAKindHand("121J1");
    TestClassify(threeOfAKindHand, HandType::ThreeOfAKind);

    std::string twoPairHand("2525J");
    TestClassify(twoPairHand, HandType::TwoPair);

    std::string onePairHand("12J3J");
    TestClassify(onePairHand, HandType::OnePair);

    std::string highCardHand("54321");
    TestClassify(highCardHand, HandType::HighCard);
}

void TestCardIndexFinder() {
    if (GetCardIndex('2') != 0)
        throw "Index 2 failed";

    if (GetCardIndex('A') != 12)
        throw "Index A failed.";

    std::cout << "TestCardIndexFinder SUCCESS";
}

void TestCardSort() {
    Hand testHandA("KKKKK", 100);
    Hand testHandB("AAAA2", 200);
    Hand testHandC("22233", 300);

    std::vector<Hand> hands { testHandA, testHandB, testHandC };
    std::sort(hands.begin(), hands.end(), SortTwoHands);

    bool result = SortTwoHands(testHandB, testHandA);
    std::cout << "Sort test result: " << result << std::endl;
}

int main()
{
    std::cout << "Advent of Code 2023 - Day 07!\n";

    //TestClassifier();
    //TestCardIndexFinder();
    //TestCardSort();
    

    //std::ifstream ifs("test_input.txt");
    std::ifstream ifs("input.txt");

    std::string line;
    std::regex handPattern(R"(([\d\w]{5})\s+(\d+))");
    std::vector<Hand> hands;
    while (ifs.good()) {
        std::getline(ifs, line);
        std::smatch match;
        std::regex_search(line, match, handPattern);

        std::string cards = match[1];
        int bid = std::stoi(match[2]);
        Hand newHand(cards, bid);
        hands.push_back(newHand);
    }

    std::sort(hands.begin(), hands.end(), SortTwoHands);

    // Calculate total score
    long long totalScore = 0;
    for (int i = 0; i < hands.size(); i++) {
        totalScore += hands[i].bid * (i + 1);
    }

    std::cout << "PART 1 ANSWER - Total score of sorted hands: " << totalScore << std::endl;

    return 0;
}