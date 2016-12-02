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

    char num_pad[5][5] = {{'0','0','1','0','0'}, {'0','2','3','4','0'}, {'5','6','7','8','9'}, {'0','A','B','C','0'}, {'0','0','D','0','0'}};
    int x=0, y=2;
    std::string code{};

    for (const std::string& line : lines) {
        for (const char& c : line) {
            switch(c) {
                case 'U':
                    if (y > 0) --y;
                    if (num_pad[y][x] == '0') ++y;
                    break;
                case 'D':
                    if (y < 4) ++y;
                    if (num_pad[y][x] == '0') --y;
                    break;
                case 'L':
                    if (x > 0) --x;
                    if (num_pad[y][x] == '0') ++x;
                    break;
                case 'R':
                    if (x < 4) ++x;
                    if (num_pad[y][x] == '0') --x;
                    break;
            }
        }
        code += num_pad[y][x];
    }

    std::cout << code << std::endl;

    return 0;
}