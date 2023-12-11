#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <regex>
#include <map>

using std::cout;

int GetCardIndex(char label, bool useJokers) {
    if (!useJokers) {
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
    else {
        switch (label) {
        case 'J': return 0;
        case '2': return 1;
        case '3': return 2;
        case '4': return 3;
        case '5': return 4;
        case '6': return 5;
        case '7': return 6;
        case '8': return 7;
        case '9': return 8;
        case 'T': return 9;
        case 'Q': return 10;
        case 'K': return 11;
        case 'A': return 12;
        default: throw "Unrecognized card index: " + label;
        }
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

static std::string GetHandTypeName(const HandType& handType) {
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

bool SortTwoCards(char left, char right, bool useJokers) {
    return GetCardIndex(left, useJokers) > GetCardIndex(right, useJokers);
}

HandType ClassifyHand(const std::string& cards, bool useJokers) {
    if (!useJokers) {

        // Sort a copy of the input first (simplifies the regex work to have similar cards grouped)
        std::string input = cards;
        std::sort(input.begin(), input.end(),
            [](char leftCard, char rightCard) {
                return SortTwoCards(leftCard, rightCard, false);
            });

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
    else {
        // Using jokers, regex patterns are more complicated
        std::string input = cards;
        std::sort(input.begin(), input.end(), [](char left, char right) {
            return SortTwoCards(left, right, true);
            });

        std::regex fiveOfAKindPattern(R"((\w)(?:\1|J)(?:\1|J)(?:\1|J)(?:\1|J))");
        if (std::regex_search(input, fiveOfAKindPattern)) {
            return HandType::FiveOfAKind;
        }

        std::regex fourOfAKindPattern(R"((\w).*(?:\1|J).*(?:\1|J).*(?:\1|J))");
        if (std::regex_search(input, fourOfAKindPattern)) {
            return HandType::FourOfAKind;
        }

        std::regex fullHousePattern(R"((?:(\w)\1\1(\w)(?:\2|J))|(?:(\w)\3(\w)(?:\4|J)(?:\4|J)))");
        if (std::regex_search(input, fullHousePattern)) {
            return HandType::FullHouse;
        }

        std::regex threeOfAKindPattern(R"(.*(\w)(?=.*(?:\1|J).*(?:\1|J)))");
        if (std::regex_search(input, threeOfAKindPattern)) {
            return HandType::ThreeOfAKind;
        }

        std::regex twoPairPattern(R"((?:.*(\w)(?=.*(?:\1|J)).*(\w)(?=.*(?:\2)))|(?:.*(\w)(?=.*(?:\3)).*(\w)(?=.*(?:\4|J))))");
        if (std::regex_search(input, twoPairPattern)) {
            return HandType::TwoPair;
        }

        std::regex onePairPattern(R"(.*(\w).*(\1|J))");
        if (std::regex_search(input, onePairPattern)) {
            return HandType::OnePair;
        }

        return HandType::HighCard;
    }
}

bool SortTwoCardsWithJokers(char left, char right) {
    if (left == 'J')
        return true;

    return left < right;
}

void TestClassify(std::string& cards, HandType expectedResult) {
    HandType result = ClassifyHand(cards, false);
    std::string matchesExpectedResult = expectedResult == result ? "CORRECT" : "WRONG";
    std::cout << "TestClassify: " << cards << " -> " << GetHandTypeName(result) << ", Matches expected result?: " <<  matchesExpectedResult << std::endl;
}

bool SortTwoHands(Hand& leftHand, Hand& rightHand, bool useJokers) {
    HandType leftClass = ClassifyHand(leftHand.cards, useJokers);
    HandType rightClass = ClassifyHand(rightHand.cards, useJokers);
    int leftIndex = static_cast<int>(leftClass);
    int rightIndex = static_cast<int>(rightClass);

    if (leftIndex < rightIndex)
        return true;

    if (leftIndex > rightIndex)
        return false;

    if (leftIndex == rightIndex) {
        for (int i = 0; i < leftHand.cards.length(); i++) {
            int leftCardIndex = GetCardIndex(leftHand.cards[i], useJokers);
            int rightCardIndex = GetCardIndex(rightHand.cards[i], useJokers);
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
    if (GetCardIndex('2', false) != 0)
        throw "Index 2 failed";

    if (GetCardIndex('A', false) != 12)
        throw "Index A failed.";

    std::cout << "TestCardIndexFinder SUCCESS";
}

void TestCardSort() {
    Hand testHandA("KKKKK", 100);
    Hand testHandB("AAAA2", 200);
    Hand testHandC("22233", 300);

    std::vector<Hand> hands { testHandA, testHandB, testHandC };
    std::sort(hands.begin(), hands.end(), [](auto& left, auto& right) {return SortTwoHands(left, right, false); });

    bool result = SortTwoHands(testHandB, testHandA, false);
    std::cout << "Sort test result: " << result << std::endl;
}

void SolvePart1(std::vector<Hand>& hands) {
    std::vector<Hand> sortedHands = hands;
    std::sort(sortedHands.begin(), sortedHands.end(), [](auto& left, auto& right) {return SortTwoHands(left, right, false); });

    // Calculate total score
    long long totalScore = 0;
    for (int i = 0; i < sortedHands.size(); i++) {
        totalScore += sortedHands[i].bid * (i + 1);
    }

    std::cout << "PART 1 ANSWER - Total score of sorted hands: " << totalScore << std::endl;
}

void SolvePart2(std::vector<Hand>& hands) {
    // Part 2 - Now with Jokers (wild cards)
    std::vector<Hand> sortedHandsWithJokers = hands;
    std::sort(sortedHandsWithJokers.begin(), sortedHandsWithJokers.end(), [](Hand& left, Hand& right) {
        return SortTwoHands(left, right, true);
        });

    int index = 0;
    for (const Hand& hand : sortedHandsWithJokers) {
        auto& cards = hand.cards;
        std::cout << index << " - " << hand.cards << " - " << GetHandTypeName(ClassifyHand(cards, true)) << ", bid " << hand.bid << std::endl;
        index++;
    }
    
    // Calculate total score
    long long totalScoreWithJokers = 0;
    for (int i = 0; i < sortedHandsWithJokers.size(); i++) {
        totalScoreWithJokers += sortedHandsWithJokers[i].bid * (i + 1);
    }

    std::cout << "PART 2 ANSWER - Total score of sorted hands including jokers: " << totalScoreWithJokers << std::endl;
}

void ExpectClassification(const Hand& hand, const HandType expectedHandType, bool useJokers) {
    auto result = ClassifyHand(hand.cards, useJokers);
    if (result != expectedHandType) {
        std::cout << "Given hand " << hand.cards << ", expected " << GetHandTypeName(expectedHandType) << " but got " << GetHandTypeName(result) << std::endl;
        throw;
    }
}

int main()
{
    std::cout << "Advent of Code 2023 - Day 07!\n";

    //Hand handA("J39Q5", 100);
    //Hand handB("2398Q", 200);
    //Hand handC("24972", 300);
    //Hand handD("2T7T7", 400);
    //Hand handE("JT7T7", 500);
    //Hand handF("TT7TJ", 500); // Was fullhouse should be 4ofakind
    //std::vector<Hand> testHands{ handA, handB, handC, handD, handE };
    //std::sort(testHands.begin(), testHands.end(), [](auto& left, auto& right) {return SortTwoHands(left, right, true); });
    ////bool result = SortTwoHands(handA, handB, true);
    /*ExpectClassification(handA, HandType::OnePair, true);
    ExpectClassification(handB, HandType::HighCard, true);
    ExpectClassification(handC, HandType::OnePair, true);
    ExpectClassification(handD, HandType::TwoPair, true);
    ExpectClassification(handE, HandType::FullHouse, true);*/
    //return 0;

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

    //SolvePart1(hands);
    SolvePart2(hands);
    
    return 0;
}