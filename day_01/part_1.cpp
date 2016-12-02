#include <fstream>
#include <complex>
#include <iostream>
#include <string>
#include <regex>

int main() {
    std::ifstream file("input.txt");
    std::string line = "";
    std::getline(file, line);

    std::regex reg("([RL])([0-9]+)");
    std::sregex_iterator match = std::sregex_iterator(line.cbegin(), line.cend(), reg);

    std::complex<float> orientation{0, 1};
    std::complex<float> position{0, 0};

    for (; match != std::sregex_iterator(); ++match) {
        if ((*match)[1] == 'R') {
            orientation *= std::complex<float>{0,-1};
        } else if ((*match)[1] == 'L') {
            orientation *= std::complex<float>{0, 1};
        }
        position += orientation * std::stof((*match)[2]);
    }

    std::cout << std::abs(position.real()) + std::abs(position.imag()) << std::endl;

    return 0;
}