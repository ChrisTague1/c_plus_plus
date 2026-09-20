#include <iostream>
#include <vector>
#include <cmath>

void find_target(const std::vector<int>& arr, int target) {
    if (arr.size() < 2) {
        return;
    }
    
    int left = 0;
    int right = arr.size() - 1;
    int total = arr[left] + arr[right];
    int best = std::abs(total - target);
    int best_left = 0;
    int best_right = arr.size() - 1;

    while (right - left > 1) {
        if (total >  target) {
            --right;
        } else if (total < target) {
            ++left;
        } else {
            break;
        }

        total = arr[left] + arr[right];
        int diff = std::abs(total - target);

        if (diff < best) {
            best = diff;
            best_left = left;
            best_right = right;
        }
    }

    std::cout << "(" << best_left << ", " << best_right << ")" << std::endl;
}

struct TestCase {
    std::vector<int> arr;
    int target;
};

int main() {
    std::vector<TestCase> test_cases = {
        {
            {-4, 1, 6, 8, 11},
            13
        }
    };

    for (const auto& test_case : test_cases) {
        std::vector<int> arr = test_case.arr;
        int target = test_case.target;

        find_target(arr, target);
    }

    return 0;
}