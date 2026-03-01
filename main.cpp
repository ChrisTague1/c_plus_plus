#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

std::optional<int> extract_num(std::string str) {
    std::optional<int> num = std::nullopt;

    try {
        num = std::stoi(str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input: " << str << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Number too large" << std::endl;
    }

    return num;
}

int main() {
    std::ifstream inputFile("data.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<std::optional<int>>> sudoku;

    std::string line;

    while (std::getline(inputFile, line)) {
        std::string num_buffer = "";
        std::vector<std::optional<int>> sudoku_line;

        for (unsigned long i = 0; i < line.length(); i++) {
            if (line[i] == ' ') {
                auto num = extract_num(num_buffer);
                num_buffer = "";
                sudoku_line.push_back(num);

                continue;
            }

            num_buffer.push_back(line[i]);
        }

        auto num = extract_num(num_buffer);
        sudoku_line.push_back(num);
        sudoku.push_back(sudoku_line);
        sudoku_line.clear();
    }

    for (const auto& sudoku_line : sudoku) {
        for (const auto& num : sudoku_line) {
            std::cout << num.value_or(0);
        }

        std::cout << std::endl;
    }

    return 0;
}
