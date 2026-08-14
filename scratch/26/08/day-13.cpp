#include <iostream>
#include <vector>
#include <optional>
#include <random>
#include <cstdint>
#include <chrono>

std::optional<int> binary_search(const std::vector<int>& v, int n) {
    int left = -1;
    int right = v.size() + 1;

    while (left + 1 < right) {
        int mid = left + (right - left) / 2;
        int val = v[mid];

        if (val < n) {
            left = mid;
        } else if (val > n) {
            right = mid;
        } else {
            return mid;
        }
    }

    return std::nullopt;
}

std::optional<int> iterative_search(const std::vector<int>& v, int n) {
    int size = v.size();

    for (size_t i = 0; i < size; ++i) {
        if (n == v[i]) {
            return i;
        }
    }

    return std::nullopt;
}

struct TestCase {
    std::vector<int> v;
    std::optional<int> a;
    int n;
};

int main() {
    constexpr std::size_t data_size = 50'000'000;
    constexpr int iterative_runs = 5;
    constexpr int binary_runs = 1'000'000;
    constexpr int target = -1; // Absent, so iterative search scans everything.

    std::cout << "Building a sorted dataset of " << data_size
              << " integers (about "
              << (data_size * sizeof(int)) / (1024 * 1024) << " MiB)...\n";

    std::vector<int> data(data_size);
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<int>(i);
    }

    // Using the results makes the searches observable to an optimizing compiler.
    std::uint64_t checksum = 0;

    const auto iterative_start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterative_runs; ++i) {
        checksum += iterative_search(data, target).value_or(data.size());
    }
    const auto iterative_end = std::chrono::steady_clock::now();

    const auto binary_start = std::chrono::steady_clock::now();
    for (int i = 0; i < binary_runs; ++i) {
        checksum += binary_search(data, target).value_or(data.size());
    }
    const auto binary_end = std::chrono::steady_clock::now();

    const auto iterative_total =
        std::chrono::duration<double, std::milli>(iterative_end - iterative_start);
    const auto binary_total =
        std::chrono::duration<double, std::milli>(binary_end - binary_start);

    std::cout << "Iterative search: "
              << iterative_total.count() / iterative_runs << " ms/lookup\n";
    std::cout << "Binary search:    "
              << (binary_total.count() * 1'000'000.0) / binary_runs
              << " ns/lookup\n";
    std::cout << "Checksum: " << checksum << '\n';

    return 0;
}
