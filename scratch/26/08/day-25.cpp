#include <iostream>
#include <vector>
#include <cmath>

void find_target(const std::vector<int>& arr, int target) {
    if (arr.size() < 2) {
        return;
    }

    int best_left = 0;
    int best_right = arr.size() - 1;

    int left = best_left;
    int right = best_right;
    int best = std::abs(arr[left] + arr[right] - target);

    while (right != left) {
        int total = arr[left] + arr[right];
        int diff = std::abs(total - target);

        if (diff < best) {
            best_left = left;
            best_right = right;
            best = diff;
        }

        if (total < target) {
            ++left;    
        } else if (total > target) {
            --right;
        } else {
            break;
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