#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "lexer.h"

struct BenchResult {
    double min;
    double max;
    double avg;
};

BenchResult bench(TokenStream (*lexer_fn)(std::istream&), const std::string& content, int runs) {
    std::vector<double> times;
    times.reserve(runs);

    for (int i = 0; i < runs; ++i) {
        std::istringstream stream(content);
        auto start = std::chrono::high_resolution_clock::now();
        lexer_fn(stream);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        times.push_back(elapsed.count());
    }

    auto [min_it, max_it] = std::minmax_element(times.begin(), times.end());
    double sum = std::accumulate(times.begin(), times.end(), 0.0);

    return {*min_it, *max_it, sum / runs};
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Could not open: " << argv[1] << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    constexpr int runs = 10;

    auto r1 = bench(lexer, content, runs);
    auto r2 = bench(lexer2, content, runs);

    std::cout << "Benchmarking " << argv[1] << " (" << runs << " runs each):\n";
    std::cout.precision(3);
    std::cout << std::fixed;
    std::cout << "         min      max      avg\n";
    std::cout << "lexer    " << r1.min << "ms  " << r1.max << "ms  " << r1.avg << "ms\n";
    std::cout << "lexer2   " << r2.min << "ms  " << r2.max << "ms  " << r2.avg << "ms\n";

    return 0;
}
