// https://www.hackerrank.com/challenges/minimum-distances/problem

#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>

#include <random>
#include <chrono>

int m1(const std::vector<int>& a) {
    std::unordered_map<int, int> last_seen;
    
    int minimum = std::numeric_limits<int>::max();
    
    for (int i = 0; i < a.size(); ++i) {
        auto it = last_seen.find(a[i]);

        if (it != last_seen.end() && i - it->second < minimum) {
            minimum = i - it->second;
        }
        
        last_seen[a[i]] = i;
    }
    
    if (minimum == std::numeric_limits<int>::max()) {
        return -1;
    }
    
    return minimum;
}

int m2(const std::vector<int>& a) {
    int minimum = std::numeric_limits<int>::max();
    
    for (int i = 0; i < a.size() - 1; ++i) {
        for (int j = i + 1; j < a.size(); ++j) {
            if (a[i] == a[j] && j - i < minimum) {
                minimum = j - i;
            }
        }
    }
    
    if (minimum == std::numeric_limits<int>::max()) {
        return -1;
    }
    
    return minimum;
}

std::vector<int> gen_data(size_t n, int max_val, unsigned int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, max_val);

    std::vector<int> data(n);
    for (size_t i = 0; i < n; ++i) {
        data[i] = dist(rng);
    }

    return data;
}

int main() {
    // const size_t N = 10'000'000;
    const size_t N = 100000;
    const int MAX_VAL = 200;
    const unsigned int SEED = 0;

    auto data = gen_data(N, MAX_VAL, SEED);

    { // M1
        auto start = std::chrono::high_resolution_clock::now();
        int result = m1(data);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> dur = end - start;

        std::cout << "M1 Result: " << result << std::endl;
        std::cout << "M1 Time: " << dur.count() << " ms" << std::endl;
    }

    { // M2
        auto start = std::chrono::high_resolution_clock::now();
        int result = m2(data);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> dur = end - start;

        std::cout << "M2 Result: " << result << std::endl;
        std::cout << "M2 Time: " << dur.count() << " ms" << std::endl;
    }

    return 0;
}