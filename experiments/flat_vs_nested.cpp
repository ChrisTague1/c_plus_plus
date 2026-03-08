// flat_vs_nested.cpp
// Demonstrates why vector<vector<T>> is slow vs flat arrays.
// Quant firms care deeply about cache-friendly data layouts.
//
// Compile & run:
//   clang++ -std=c++17 -O2 -o flat_vs_nested flat_vs_nested.cpp && ./flat_vs_nested

#include <array>
#include <chrono>
#include <iostream>
#include <vector>

constexpr int N = 1000;
constexpr int ITERS = 500;

// Nested vector: each row is a separate heap allocation.
// Rows may be scattered in memory -> cache misses when iterating.
long long sum_nested(const std::vector<std::vector<int>>& grid) {
    long long total = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            total += grid[i][j];
    return total;
}

// Flat vector: one contiguous block of memory.
// Sequential access pattern -> hardware prefetcher is happy.
long long sum_flat(const std::vector<int>& grid) {
    long long total = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            total += grid[i * N + j];
    return total;
}

// std::array: same as flat but lives on the stack (no heap alloc at all).
// For fixed-size grids this is ideal.
// NOTE: large arrays on the stack can cause stack overflow — for a 9x9
// sudoku this is fine, for 1000x1000 you'd keep it on the heap.
long long sum_array(const std::array<int, N * N>& grid) {
    long long total = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            total += grid[i * N + j];
    return total;
}

template <typename Func>
double benchmark(Func fn) {
    // Warm up
    fn();

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; ++i) {
        volatile auto result = fn();  // volatile prevents optimizing away
        (void)result;
    }
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double, std::milli>(end - start).count() / ITERS;
}

int main() {
    // Setup nested
    std::vector<std::vector<int>> nested(N, std::vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            nested[i][j] = i * N + j;

    // Setup flat
    std::vector<int> flat(N * N);
    for (int i = 0; i < N * N; ++i)
        flat[i] = i;

    // Setup std::array (on heap here because 1M ints is ~4MB)
    auto arr = std::make_unique<std::array<int, N * N>>();
    for (int i = 0; i < N * N; ++i)
        (*arr)[i] = i;

    auto& arr_ref = *arr;

    double t_nested = benchmark([&]() { return sum_nested(nested); });
    double t_flat   = benchmark([&]() { return sum_flat(flat); });
    double t_array  = benchmark([&]() { return sum_array(arr_ref); });

    std::cout << "vector<vector<int>>:  " << t_nested << " ms/iter\n";
    std::cout << "vector<int> (flat):   " << t_flat   << " ms/iter\n";
    std::cout << "std::array (flat):    " << t_array  << " ms/iter\n";
    std::cout << "\nNested is ~" << t_nested / t_flat << "x slower than flat\n";

    return 0;
}
