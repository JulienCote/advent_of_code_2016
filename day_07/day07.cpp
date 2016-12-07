#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <iterator>


inline bool contains_ABBA(const std::string& input) {
    for (int i = 0; i < input.size() - 3; ++i) {
        if (input.at(i) == input.at(i+3) && input.at(i+1) == input.at(i+2) && input.at(i) != input.at(i+1)) return true;
    }
    return false;
}

inline std::vector<std::string> find_ABAs(const std::string& input) {
    std::vector<std::string> ABAs{};
    for (int i = 0; i < input.size() - 2; ++i) {
        if (input.at(i) == input.at(i+2) && input.at(i) != input.at(i+1)){
            ABAs.push_back(input.substr(i, 3));
        }
    }
    return ABAs;
}

inline bool contains_BAB_in_ABAs(const std::vector<std::string>& supernet_ABAs, const std::vector<std::string>& hypernet_ABAs) {
    for (auto s_aba : supernet_ABAs) {
        for (auto h_aba : hypernet_ABAs) {
            if (s_aba.at(0) == h_aba.at(1) && h_aba.at(0) == s_aba.at(1))  return true;
        }
    }
    return false;
}


int main() {
    std::ifstream file("input.txt");
    std::string line = "";
    std::vector<std::vector<std::string>> split_inputs{{}};
    std::regex reg("([a-z]+)");
    int line_number = 0;
    while(std::getline(file, line)) {
        std::sregex_iterator begin = std::sregex_iterator(line.cbegin(), line.cend(), reg);
        std::sregex_iterator end = std::sregex_iterator();
        split_inputs.emplace_back(std::vector<std::string>{});

        for (std::sregex_iterator word = begin; word != end; ++word) {
                split_inputs.at(line_number).push_back((*word).str());
        }
        ++line_number;
    }
    file.close();

    int ABBA_count = 0;
    int SSL_count = 0;
    for (const std::vector<std::string>& line : split_inputs) {
        bool abba_present = false;
        bool hypernet_sequence_present = false;
        std::vector<std::string> supernet_ABAs{};
        std::vector<std::string> hypernet_ABAs{};
        for (int i = 0; i < line.size(); ++i) {
            std::vector<std::string> word_ABAs = find_ABAs(line.at(i));
            if (i % 2 == 1) {
                hypernet_ABAs.insert(hypernet_ABAs.end(), word_ABAs.begin(), word_ABAs.end());
                if (contains_ABBA(line.at(i))) {
                    hypernet_sequence_present = true;
                }
            } else {
                supernet_ABAs.insert(supernet_ABAs.end(), word_ABAs.begin(), word_ABAs.end());
                if (contains_ABBA(line.at(i))) {
                    abba_present = true;
                }
            }
        }

        if (abba_present && !hypernet_sequence_present) ++ABBA_count;
        if (contains_BAB_in_ABAs(supernet_ABAs, hypernet_ABAs)) ++SSL_count;
    }

    std::cout << "ABBA count: " << ABBA_count << std::endl;
    std::cout << "SSL count: " << SSL_count << std::endl;

    return 0;
}
