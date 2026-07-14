#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <array>
#include <cstdint>

#include "parsing/extract_num.h"

constexpr int SUDOKU_ROOT = 3;
constexpr int SUDOKU_SIZE = SUDOKU_ROOT * SUDOKU_ROOT;
constexpr uint16_t ALL_CANDIDATES = (1 << SUDOKU_SIZE) - 1; // bits 0..8 = values 1..9

using Sudoku = std::array<char, SUDOKU_SIZE * SUDOKU_SIZE>;

// Bitmasks: bit i set means value (i+1) is used in that row/col/box
struct Constraints {
    uint16_t rows[SUDOKU_SIZE]{};
    uint16_t cols[SUDOKU_SIZE]{};
    uint16_t boxes[SUDOKU_SIZE]{};

    void set(int r, int c, int val) {
        uint16_t bit = 1 << (val - 1);
        rows[r] |= bit;
        cols[c] |= bit;
        boxes[(r / SUDOKU_ROOT) * SUDOKU_ROOT + c / SUDOKU_ROOT] |= bit;
    }

    void clear(int r, int c, int val) {
        uint16_t bit = ~(1 << (val - 1));
        rows[r] &= bit;
        cols[c] &= bit;
        boxes[(r / SUDOKU_ROOT) * SUDOKU_ROOT + c / SUDOKU_ROOT] &= bit;
    }

    uint16_t candidates(int r, int c) const {
        return ALL_CANDIDATES & ~(rows[r] | cols[c] | boxes[(r / SUDOKU_ROOT) * SUDOKU_ROOT + c / SUDOKU_ROOT]);
    }
};

int extract_sudoku(const std::string_view filename, Sudoku& sudoku) {
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

// Empty cell indices, pre-collected to avoid scanning the board
struct EmptyCells {
    int indices[SUDOKU_SIZE * SUDOKU_SIZE];
    int count = 0;
};

bool solve(Sudoku& sudoku, Constraints& con, EmptyCells& empties, int pos) {
    if (pos >= empties.count) return true;

    // MRV: find the empty cell (from pos onward) with fewest candidates
    int best = pos;
    int best_count = 10;
    for (int e = pos; e < empties.count; e++) {
        int idx = empties.indices[e];
        int r = idx / SUDOKU_SIZE;
        int c = idx % SUDOKU_SIZE;
        int cnt = __builtin_popcount(con.candidates(r, c));
        if (cnt < best_count) {
            best_count = cnt;
            best = e;
            if (cnt <= 1) break; // can't do better than 0 or 1
        }
    }

    if (best_count == 0) return false;

    // Swap best to current position
    std::swap(empties.indices[pos], empties.indices[best]);

    int idx = empties.indices[pos];
    int r = idx / SUDOKU_SIZE;
    int c = idx % SUDOKU_SIZE;
    uint16_t cands = con.candidates(r, c);

    // Iterate only valid candidates using bit manipulation
    while (cands) {
        int bit = cands & (-cands); // lowest set bit
        cands &= cands - 1;        // clear it
        int val = __builtin_ctz(bit) + 1;

        sudoku[idx] = val;
        con.set(r, c, val);

        if (solve(sudoku, con, empties, pos + 1)) return true;

        con.clear(r, c, val);
    }

    sudoku[idx] = 0;

    // Swap back for correct backtracking
    std::swap(empties.indices[pos], empties.indices[best]);

    return false;
}

int solve_sudoku(Sudoku& sudoku) {
    Constraints con{};
    EmptyCells empties{};

    for (int i = 0; i < SUDOKU_SIZE * SUDOKU_SIZE; i++) {
        if (sudoku[i] != 0) {
            con.set(i / SUDOKU_SIZE, i % SUDOKU_SIZE, sudoku[i]);
        } else {
            empties.indices[empties.count++] = i;
        }
    }

    return solve(sudoku, con, empties, 0) ? 0 : -1;
}

void print_sudoku(Sudoku& sudoku) {
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            std::cout << static_cast<int>(sudoku[i * SUDOKU_SIZE + j]) << " ";
        }

        std::cout << "\n";
    }
}

// constexpr long int ITERATIONS = 100'000'000;
constexpr long int ITERATIONS = 100;

int main() {
    Sudoku sudoku;

    if (extract_sudoku("data.txt", sudoku)) {
        return 1;
    }

    print_sudoku(sudoku);

    Sudoku original = sudoku;

    auto start = std::chrono::high_resolution_clock::now();

    for (long int iter = 0; iter < ITERATIONS; iter++) {
        sudoku = original;
        if (solve_sudoku(sudoku)) {
            std::cerr << "Could not solve sudoku" << std::endl;
            return 2;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\nFound solution (" << ITERATIONS << " iterations in "
              << duration.count() << " ms)..." << "\n\n";

    print_sudoku(sudoku);

    return 0;
}
