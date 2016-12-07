#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <array>

int main() {
	std::ifstream file("input.txt");
	std::string line = "";
	std::vector<std::array<int, 3>> input{};

	std::regex reg("(\\d+)\\s+(\\d+)\\s+(\\d+)");
	std::sregex_iterator match = std::sregex_iterator(line.cbegin(), line.cend(), reg);

	while (std::getline(file, line)) {
		for (; match != std::sregex_iterator(); ++match) {
			input.push_back({ std::stoi((*match)[1]), std::stoi((*match)[2]), std::stoi((*match)[3]) });
		}
	}
	file.close();
	
	int count = 0;
	for (int i = 0; i < input.size(); i += 3) {
		const std::array<int, 3>& first = input.at(i);
		const std::array<int, 3>& second = input.at(i+1);
		const std::array<int, 3>& third = input.at(i+2);

		for (int j = 0; j < 3; ++j) {
			if (first[i] + second[i] > third[i] &&
				second[i] + third[i]  > first[i] &&
				third[i] + first[i]  > second[i])
				++count;
		}
	}

	std::cout << count << std::endl;

	return 0;
}
