#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <array>

void rect(const int x, const int y, std::array<std::array<bool, 50>, 6>& screen) {
	for (int i = 0; i < y; ++i) {
		for (int j = 0; j < x; ++j) {
			screen.at(i).at(j) = true;
		}
	}
}

void rotate_column(int id, int amplitude, std::array<std::array<bool, 50>, 6>& screen) {
	std::array<bool, 6> new_column{};
	for (int i = 0; i < 6; ++i) {
		new_column.at((i + amplitude) % 6) = screen.at(i).at(id);
	}
	for (int i = 0; i < 6; ++i) {
		screen.at(i).at(id) = new_column.at(i);
	}
}

void rotate_row(int id, int amplitude, std::array<std::array<bool, 50>, 6>& screen) {
	std::rotate(screen.at(id).begin(), screen.at(id).begin()+(50-amplitude), screen.at(id).end());
}

void print_screen(std::array<std::array<bool, 50>, 6>& screen) {
	for (auto r : screen) {
		for (auto p : r) {
			if (p) std::cout << '#';
			else std::cout << '.';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	std::ifstream file("input.txt");
	std::string line{};
	std::array<std::array<bool, 50>, 6> screen{ {} };

	while (std::getline(file, line)) {
		std::regex pattern("([a-z]+) ([a-z]+)? ?([0-9]*).?[x=]([0-9]+)[a-z, ]*([0-9]+)?");
		std::sregex_iterator begin = std::sregex_iterator(line.cbegin(), line.cend(), pattern);
		std::sregex_iterator end{};

		for (; begin != end; ++begin) {
			if ((*begin)[1] == "rect") {
				rect(std::stoi((*begin)[3]), std::stoi((*begin)[4]), screen);
			}
			else if ((*begin)[1] == "rotate") {
				if ((*begin)[2] == "row") {
					rotate_row(std::stoi((*begin)[4]), std::stoi((*begin)[5]), screen);
				} else if ((*begin)[2] == "column") {
					rotate_column(std::stoi((*begin)[4]), std::stoi((*begin)[5]), screen);
				}
			}
		}
	}

	print_screen(screen);

	int count = 0;
	for (auto r : screen) {
		for (auto p : r) {
			if (p) ++count;
		}
	}

	std::cout << count << std::endl;

	return 0;
}