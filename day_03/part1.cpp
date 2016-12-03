#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>


int main() {
	std::ifstream file("input.txt");
	std::string line = "";

	int count = 0;

	while (std::getline(file, line)) {
		std::regex reg("(\\d+)\\s+(\\d+)\\s+(\\d+)");
		std::sregex_iterator match = std::sregex_iterator(line.cbegin(), line.cend(), reg);

		for (; match != std::sregex_iterator(); ++match) {
			int sides[3] = { std::stoi((*match)[1]), std::stoi((*match)[2]), std::stoi((*match)[3]) };

			if (sides[0] + sides[1] > sides[2] &&
				sides[1] + sides[2] > sides[0] &&
				sides[2] + sides[0] > sides[1])
				count++;
		}
	}
	file.close();
	std::cout << count << std::endl;
	std::cin.get();

	return 0;
}