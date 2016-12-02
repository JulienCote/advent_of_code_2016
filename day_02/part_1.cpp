#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines{};

    { //File read stuff
        std::ifstream file("input.txt");
        std::string line = "";
        while(std::getline(file, line)) {
            lines.push_back(line);
        }
    }

    char num_pad[3][3] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};
    int x=1, y=1;
    std::string code{};

    for (const std::string& line : lines) {
        for (const char& c : line) {
            switch(c) {
                case 'U': if (y > 0) --y; break;
                case 'D': if (y < 2) ++y; break;
                case 'L': if (x > 0) --x; break;
                case 'R': if (x < 2) ++x; break;
            }
        }
        code += num_pad[y][x];
    }

    std::cout << code << std::endl;

    return 0;
}