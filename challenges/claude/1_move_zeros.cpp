// CHALLENGE: Move Zeros
//
// Given a vector of integers, move all zeros to the end while maintaining
// the relative order of the non-zero elements. Do this IN-PLACE.
//
// Example: {0, 1, 0, 3, 12} -> {1, 3, 12, 0, 0}
//
// Hint: Think about vector::erase() and vector::push_back(), or
// use iterators to swap elements in place.

#include <iostream>
#include <vector>

using namespace std;

void moveZeros2(vector<int>& nums) {
    int writes = 0;
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] != 0) {
            nums[writes++] = nums[i];
        }
    }
    std::fill(nums.begin() + writes, nums.end(), 0);
}

void moveZeros(vector<int>& nums) {
    int zeros = 0;

    std::erase_if(nums, [&](int a) {
        bool result = a == 0;

        if (result) {
            ++zeros;
        }

        return result;
    });

    for (int i = 0; i < zeros; i++) {
        nums.push_back(0);
    }
}

bool test(vector<int> input, vector<int> expected, int testNum) {
    moveZeros(input);
    bool pass = (input == expected);
    cout << "Test " << testNum << ": " << (pass ? "PASS" : "FAIL") << endl;
    if (!pass) {
        cout << "  Expected: ";
        for (int x : expected) cout << x << " ";
        cout << "\n  Got:      ";
        for (int x : input) cout << x << " ";
        cout << endl;
    }
    return pass;
}

int main() {
    int passed = 0, total = 5;

    passed += test({0, 1, 0, 3, 12}, {1, 3, 12, 0, 0}, 1);
    passed += test({0, 0, 0}, {0, 0, 0}, 2);
    passed += test({1, 2, 3}, {1, 2, 3}, 3);
    passed += test({0}, {0}, 4);
    passed += test({4, 0, 1, 0, 0, 2}, {4, 1, 2, 0, 0, 0}, 5);

    cout << "\n" << passed << "/" << total << " tests passed." << endl;
    return passed == total ? 0 : 1;
}
