// CHALLENGE: Two Sum
//
// Given a vector of integers and a target sum, return a vector containing
// the TWO INDICES of elements that add up to the target. You may assume
// exactly one valid pair exists. Return the indices in ascending order.
//
// Example: nums = {2, 7, 11, 15}, target = 9 -> {0, 1}  (because 2+7=9)
//
// Hint: Use a map to store each number's index as you iterate.
// For each element, check if (target - current) already exists in the map
// using map::find() or map::count(). This gives you O(n) instead of O(n^2).

#include <iostream>
#include <vector>
#include <map>

using namespace std;

vector<int> twoSum(const vector<int>& nums, int target) {
    // YOUR CODE HERE
    return {};
}

bool test(vector<int> nums, int target, vector<int> expected, int testNum) {
    vector<int> result = twoSum(nums, target);
    bool pass = (result == expected);
    cout << "Test " << testNum << ": " << (pass ? "PASS" : "FAIL") << endl;
    if (!pass) {
        cout << "  Expected: ";
        for (int x : expected) cout << x << " ";
        cout << "\n  Got:      ";
        for (int x : result) cout << x << " ";
        cout << endl;
    }
    return pass;
}

int main() {
    int passed = 0, total = 5;

    passed += test({2, 7, 11, 15}, 9, {0, 1}, 1);
    passed += test({3, 2, 4}, 6, {1, 2}, 2);
    passed += test({1, 5, 3, 7}, 8, {1, 3}, 3);
    passed += test({0, 4, 3, 0}, 0, {0, 3}, 4);
    passed += test({-1, 10, 3, -4}, -5, {0, 3}, 5);

    cout << "\n" << passed << "/" << total << " tests passed." << endl;
    return passed == total ? 0 : 1;
}
