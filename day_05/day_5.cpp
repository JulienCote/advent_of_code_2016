#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mutex>

#include "md5/hashlibpp.h"

inline bool is_hash_valid(const std::string& hash) {
	for (int i = 0; i < 5; ++i) {
		if (hash.at(i) != '0') return false;
	}
	return true;
}

int main() {
	std::string input = "wtnhxymk";
	std::string part1 = "";
	std::string part2 = "________";
	std::string hash = "";
	unsigned int salt = 0;	
	int part2_found = 0;
	md5wrapper md5;

	while (part2_found < 8) {
		 hash = md5.getHashFromString(input + std::to_string(salt));
		if (is_hash_valid(hash)) {
			char& fifth = hash.at(5);
			int fifth_int = fifth - '0';			
			if (fifth_int >= 0 && fifth_int <= 7 && part2.at(fifth_int) == '_') {
				part2_found++;
				part2.at(fifth_int) = hash.at(6);
				std::cout << "part 2: " << part2 << std::endl;
			}
			if (part1.size() < 8) {
				part1 += fifth;
				std::cout << "part 1: " << part1 << std::endl;
			}			
		}
		salt++;
	}
	return 0;
}