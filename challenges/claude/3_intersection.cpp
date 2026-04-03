// CHALLENGE: Array Intersection
//
// Given two vectors of integers, return a SORTED vector of elements
// that appear in BOTH vectors. Each common element should appear only
// once in the output, even if it appears multiple times in the inputs.
//
// Example: {1, 2, 2, 3} and {2, 3, 4} -> {2, 3}
//
// Hint: Insert elements from each vector into their own set (this
// deduplicates automatically), then iterate one set and use
// set::find() or set::count() to check membership in the other.

#include <iostream>
#include <vector>
#include <set>

using namespace std;

vector<int> intersection(const vector<int>& a, const vector<int>& b) {
    set<int> setA;
    set<int> setB;

    for (const auto& item : a) {
        setA.insert(item);
    }

    for (const auto& item : b) {
        setB.insert(item);
    }

    vector<int> output;

    for (const auto& item : setA) {
        if (setB.contains(item)) {
            output.emplace_back(item);
        }
    }

    return output;
}

bool test(vector<int> a, vector<int> b, vector<int> expected, int testNum) {
    vector<int> result = intersection(a, b);
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

    passed += test({1, 2, 2, 3}, {2, 3, 4}, {2, 3}, 1);
    passed += test({1, 2, 3}, {4, 5, 6}, {}, 2);
    passed += test({5, 5, 5}, {5}, {5}, 3);
    passed += test({}, {1, 2, 3}, {}, 4);
    passed += test({10, 3, 7, 1}, {7, 1, 20, 3}, {1, 3, 7}, 5);

    cout << "\n" << passed << "/" << total << " tests passed." << endl;
    return passed == total ? 0 : 1;
}
