#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

#include "parsing/extract_num.h"

constexpr int SUDOKU_ROOT = 2;
constexpr int SUDOKU_SIZE = SUDOKU_ROOT * SUDOKU_ROOT;

using Sudoku = std::array<int, SUDOKU_SIZE * SUDOKU_SIZE>;

int extract_sudoku(const std::string_view filename, Sudoku& sudoku) { // todo no error handling
    std::ifstream inputFile(filename.data());

    if (!inputFile.is_open()) {
        std::cerr << "Error: Sudoku file could not be opened!" << std::endl;
        return 1;
    }

    int i = 0;

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string temp;

        while (ss >> temp && i < SUDOKU_SIZE * SUDOKU_SIZE) {
            sudoku[i++] = extract_num(temp);
        }
    }

    inputFile.close();

    return 0;
}

bool value_works(const Sudoku& sudoku, int value, int i) {
    std::cout << "Checking rows" << "\n";
    int row_ind = i / SUDOKU_SIZE;
    int row = row_ind * SUDOKU_SIZE;
    for (int x = row; x < row + SUDOKU_SIZE; x++) {
        if (sudoku[x] == value) {
            return false;
        }
    }

    std::cout << "Checking cols" << "\n";
    int col_ind = i % SUDOKU_SIZE;
    for (int x = 0; x < SUDOKU_SIZE; x++) {
        if (sudoku[x * SUDOKU_SIZE + col_ind] == value) {
            return false;
        }
    }

    int row_sqr = (row_ind / SUDOKU_ROOT) * SUDOKU_ROOT;
    int col_sqr = (col_ind / SUDOKU_ROOT) * SUDOKU_ROOT;

    for (int x = 0; x < SUDOKU_ROOT; x++) {
        row = row_sqr + x;

        for (int y = 0; y < SUDOKU_ROOT; y++) {
            int col = col_sqr + y;

            if (sudoku[row * SUDOKU_SIZE + col] == value) {
                return false;
            }
        }
    }

    return true;
}

int solve_sudoku(Sudoku& sudoku) {
    for (int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        if (sudoku[i] == 0) {
            for (int v = 1; v <= SUDOKU_SIZE; v++) {
                std::cout << "Checking " << v << " at " << i << "\n";
                if (value_works(sudoku, v, i)) {
                    sudoku[i] = v;

                    if (!solve_sudoku(sudoku)) {
                        return 0;
                    }
                }
            }

            return -1;
        }
    }

    return 0;
}

void print_sudoku(Sudoku& sudoku) {
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            std::cout << sudoku[i * SUDOKU_SIZE + j] << " ";
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
