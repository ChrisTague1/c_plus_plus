#include <iostream>
#include <vector>
#include <cmath>

/*
0   1  2  3   4
               
-4, 1, 6, 8, 11
               
*/

void find_target(const std::vector<int>& arr, int target) {
    if (arr.size() < 2) {
        return;
    }

    int left = 0;
    int right = 1;
    int best = std::abs(arr[left] + arr[right] - target);

    for (int i = 0; i < arr.size() - 1; ++i) {
        for (int j = i + 1; j < arr.size(); ++j) {
            int total = arr[i] + arr[j];
            int diff = std::abs(total - target);

            if (diff < best) {
                best = diff;
                left = i;
                right = j;
            }
        }
    }

    std::cout << "(" << left << ", " << right << ")" << std::endl;
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