// CHALLENGE: Remove Duplicates (Preserve Order)
//
// Given a vector of integers, return a NEW vector with duplicates removed.
// The first occurrence of each element should be kept, and the relative
// order must be preserved.
//
// Example: {3, 1, 3, 2, 1} -> {3, 1, 2}
//
// Hint: A set is great for tracking what you've already seen.
// Use set::insert() or set::count()/set::find() alongside
// vector::push_back() to build the result.

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

vector<int> removeDuplicates(const vector<int>& nums) {
    unordered_set<int> uniqueTracker;
    vector<int> output;

    for (const auto& num : nums) {
        auto it = uniqueTracker.find(num);

        if (it != uniqueTracker.end())
            continue;
        
        uniqueTracker.emplace(num);
        output.emplace_back(num);
    }

    return output;
}

bool test(vector<int> input, vector<int> expected, int testNum) {
    vector<int> result = removeDuplicates(input);
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

    passed += test({3, 1, 3, 2, 1}, {3, 1, 2}, 1);
    passed += test({1, 1, 1, 1}, {1}, 2);
    passed += test({5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}, 3);
    passed += test({}, {}, 4);
    passed += test({7, 7, 8, 8, 7, 9}, {7, 8, 9}, 5);

    cout << "\n" << passed << "/" << total << " tests passed." << endl;
    return passed == total ? 0 : 1;
}
