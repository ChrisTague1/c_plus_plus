#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <charconv>
#include <sstream>

std::optional<int> extract_num(std::string_view str) {
    int value;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

    if (ec == std::errc()) { // std::errc() is a default constructor, it is an enum std::errc::stuff_here
        return value;
    }

    return std::nullopt;
}

int main() {
    std::ifstream inputFile("data.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<std::optional<int>>> sudoku;
    sudoku.reserve(4); // this one is 4x4

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;

        std::vector<std::optional<int>> row;
        row.reserve(4);

        std::stringstream ss(line);
        std::string temp;
        while (ss >> temp) {
            row.push_back(extract_num(temp));
        }

        sudoku.push_back(std::move(row));
    }

    for (const auto& row : sudoku) {
        for (const auto& cell : row) {
            std::cout << cell.value_or(0) << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
