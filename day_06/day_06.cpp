#include <iostream>
#include <future>
#include <string>
#include <fstream>
#include <array>
#include <iterator>

struct ColumnInfo {
	char most_common = '\0';
	char least_common = '\0';
};

ColumnInfo column_process(std::array<int, 26>::iterator begin, std::array<int, 26>::const_iterator end) {	
	int max_value = 0, min_value = 26;
	ColumnInfo info;
	for (char i = 0; begin != end; ++begin) {		
		if (*begin > max_value) {
			max_value = *begin;
			info.most_common = i;
		}
		else if (*begin < min_value) {
			min_value = *begin;
			info.least_common = i;
		}
		++i;
	}
	info.most_common += 'a';
	info.least_common += 'a';
	return info;
}

int main() {
	std::ifstream file("input.txt");
	std::string line;	
	std::array<std::array<int, 26>, 8> columns{};

	while (std::getline(file, line)) {
		for (int i = 0; i < 8; ++i) {
			++columns.at(i).at(line.at(i) - 'a');
		}
	}
	file.close();

	std::vector<std::future<ColumnInfo>> futures{};

	for (int i = 0; i < 8; ++i) {
		futures.emplace_back(std::async(std::launch::async, column_process, columns.at(i).begin(), columns.at(i).cend()));
	}

	char password_1[9]{};
	char password_2[9]{};
	
	for (size_t i = 0; i < futures.size(); ++i) {
		ColumnInfo info = futures.at(i).get();
		password_1[i] = info.most_common;
		password_2[i] = info.least_common;
	}

	std::cout << "part 1: " << password_1 << std::endl; 
	std::cout << "part 2: " << password_2 << std::endl;

	return 0;
}