#include <fstream>
#include <complex>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>

int main() {
    std::ifstream file("input.txt");
    std::string line = "";
    std::getline(file, line);

    std::regex reg("([RL])([0-9]+)");
    std::sregex_iterator match = std::sregex_iterator(line.cbegin(), line.cend(), reg);

    std::complex<float> orientation{0, 1};
    std::complex<float> position{0, 0};

    std::vector<std::complex<float> > location_history{position};

    for (; match != std::sregex_iterator(); ++match) {
        if ((*match)[1] == 'R') {
            orientation *= std::complex<float>{0,-1};
        } else if ((*match)[1] == 'L') {
            orientation *= std::complex<float>{0, 1};
        }

        for (int i = 0; i < std::stoi((*match)[2]); ++i) {
            position += orientation * 1.f;

            if (std::find(location_history.begin(), location_history.end(), position) != location_history.end()) {
                std::cout << std::abs(position.real()) + std::abs(position.imag()) << std::endl; //FOUND IT!!!
                goto evil_goto_just_for_fun; //Mwouahaha
            }
            location_history.push_back(position);
        }
    }

    evil_goto_just_for_fun:

    return 0;
}