#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#include "parsing/extract_num.h"

using Sudoku = std::vector<std::vector<std::optional<int>>>;

const int SUDOKU_SIZE = 4;
const int SUDOKU_ROOT = static_cast<int>(std::sqrt(SUDOKU_SIZE));

int extract_sudoku(const std::string_view filename, Sudoku& sudoku) {
    std::ifstream inputFile(filename.data());

    if (!inputFile.is_open()) {
        std::cerr << "Error: Sudoku file could not be opened!" << std::endl;
        return 1;
    }

    sudoku.reserve(SUDOKU_SIZE);

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;

        std::vector<std::optional<int>> row;
        row.reserve(SUDOKU_SIZE);

        std::stringstream ss(line);
        std::string temp;
        while (ss >> temp) {
            row.push_back(extract_num(temp));
        }

        sudoku.push_back(std::move(row));
    }

    inputFile.close();

    return 0;
}

bool value_works(const Sudoku& sudoku, int value, int x, int y) {
    auto& row = sudoku[x];
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        if (row[i] == value) {
            return false;
        }
    }

    for (int i = 0; i < SUDOKU_SIZE; i++) {
        if (sudoku[i][y] == value) {
            return false;
        }
    }

    int x_square = x / SUDOKU_ROOT;
    int y_square = y / SUDOKU_ROOT;

    for (int i = x_square * SUDOKU_ROOT; i < (x_square + 1) * SUDOKU_ROOT; i++) {
        for (int j = y_square * SUDOKU_ROOT; j < (y_square + 1) * SUDOKU_ROOT; j++) {
            if (sudoku[i][j] == value) {
                return false;
            }
        }
    }

    return true;
}

int solve_sudoku(Sudoku& sudoku) {
    for (int x = 0; x < SUDOKU_SIZE; x++) {
        const auto& row = sudoku[x];

        for (int y = 0; y < SUDOKU_SIZE; y++) {
            const auto& col = row[y];

            if (!col.has_value()) {
                for (int i = 1; i <= SUDOKU_SIZE; i++) {
                    if (value_works(sudoku, i, x, y)) {
                        sudoku[x][y] = i;

                        if (!solve_sudoku(sudoku)) {
                            return 0;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

void print_sudoku(Sudoku& sudoku) {
    for (const auto& row : sudoku) {
        for (const auto& cell : row) {
            std::cout << cell.value_or(0) << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    Sudoku sudoku;

    if (extract_sudoku("data.txt", sudoku)) {
        return 1;
    }

    print_sudoku(sudoku);

    if (solve_sudoku(sudoku)) {
        std::cerr << "Could not solve sudoku" << std::endl;
        return 2;
    } else {
        std::cout << "\nFound solution..." << "\n\n";
    }

    print_sudoku(sudoku);

    return 0;
}
