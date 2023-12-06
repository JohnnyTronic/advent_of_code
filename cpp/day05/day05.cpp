#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <vector>
#include <regex>

class Transformer {
	long long outputStart, inputStart, range;

public:
	Transformer(long long outStart, long long inStart, long long range) : outputStart(outStart), inputStart(inStart), range(range) {};

	//is this input in range?
	bool IsInputInRange(long long input) {
		return input >= inputStart && input < inputStart + range;
	}

	//transform!
	long long Transform(long long input) {
		long long delta = input - inputStart;
		return outputStart + delta;
	}
};

//represents a group of transforms from one thing to another (seed to soil, etc.)
class MapperProcess {
public:
	std::vector<Transformer> transforms;

	long long MapIt(long long input) {
		for (auto& transformer : transforms) {
			if (transformer.IsInputInRange(input)) {
				return transformer.Transform(input);
			}
		}
		return input;
	}
};

class SeedToLocationConverteroTron {
public:
	std::vector<MapperProcess> mappers;

	long long SeedToLocation(long long input) {
		for (auto& mapper : mappers) {
			input = mapper.MapIt(input);
		}
		return input;
	}
};

int main() {

	std::ifstream ifs("input.txt", std::ifstream::in);
	std::string s;

	SeedToLocationConverteroTron seedToLocationConverter;
	std::vector<long long> seeds;

	std::getline(ifs, s);
	std::regex digitsPattern{ R"( (\d+))" };
	std::regex transformPattern{ R"((\d+) (\d+) (\d+))" };
	std::regex mapPattern{ R"(map:$)" };
	std::cout << "search result: " << std::regex_search(s, digitsPattern) << '\n';
	std::sregex_token_iterator iterator(s.begin(), s.end(), digitsPattern, 1);
	std::sregex_token_iterator iteratorEnd;

	while (iterator != iteratorEnd) {
		long long seed = std::stoll(*iterator);
		seeds.push_back(seed);
		std::cout << "seed: " << seed << '\n';
		iterator++;
	}

	while (ifs.good()) {
		std::getline(ifs, s);

		if (s.empty()) {
			continue;
		}

		if (std::regex_search(s, mapPattern)) {
			MapperProcess mapProcess;

			while (ifs.good()) {
				std::getline(ifs, s);
				if (s.empty()) {
					break;
				}

				std::smatch smatch;

				bool matched = std::regex_search(s, smatch, transformPattern);

				long long out = std::stoll(smatch[1]);
				long long in = std::stoll(smatch[2]);
				long long range = std::stoll(smatch[3]);
				Transformer transformer(out, in, range);
				mapProcess.transforms.push_back(transformer);
			}
			seedToLocationConverter.mappers.push_back(mapProcess);
		}
	}

	ifs.close();

	long long minLocation = seedToLocationConverter.SeedToLocation(seeds[0]);;
	for (auto& seed : seeds) {
		long long location = seedToLocationConverter.SeedToLocation(seed);
		if (location < minLocation) {
			minLocation = location;
		}
	}

	std::cout << "Part 1 - Min seed location: " << minLocation << '\n';

	// Part 2 - Seed number ranges
	std::vector<long long> rangeSeeds;
	for (int i = 0; i < seeds.size(); i += 2) {
		long long rangeStart = seeds[i];
		long long rangeSize = seeds[i + 1];
		for (int j = 0; j < rangeSize; j++) {
			rangeSeeds.push_back(rangeStart + j);
		}
	}

	long long part2MinLocation = seedToLocationConverter.SeedToLocation(rangeSeeds[0]);
	long long count;
	long long totalCount = rangeSeeds.size();
	for (auto& rangeSeed : rangeSeeds) {
		count++;
		if (count % 1000 == 0)
			std::cout << "Computed " << count << " of " << totalCount << std::endl;
		long long nextLocation = seedToLocationConverter.SeedToLocation(rangeSeed);
		if (nextLocation < part2MinLocation) {
			part2MinLocation = nextLocation;
		}
	}

	std::cout << "Part 2 - Min seed location from seed ranges: " << part2MinLocation << std::endl;

	return 0;
}