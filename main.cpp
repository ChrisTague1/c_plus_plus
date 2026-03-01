#include <iostream>
#include <fstream>
#include <string>

int main() {
    // std::ifstream inputFile = std::ifstream("data.txt");
    // std::ifstream inputFile{"data.txt"};
    std::ifstream inputFile("data.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file!" << std::endl;
        return 1;
    }

    std::string line;

    while (std::getline(inputFile, line)) {
        std::cout << line << std::endl;
    }

    return 0;
}
