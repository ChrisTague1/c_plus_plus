#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*
Problem: Count Valid Triangles

You are given a vector of non-negative integers nums, where each value represents a side length.

Return the number of index triplets (i, j, k) such that:
- i < j < k
- nums[i], nums[j], and nums[k] can form a valid triangle

A valid triangle must have all side lengths positive and satisfy the triangle inequality.

Constraints to assume:
- 0 <= nums.size() <= 3000
- 0 <= nums[i] <= 1000000
- The answer fits in a signed 64-bit integer.
*/
long long countValidTriangles(vector<int> nums) {
    // TODO: implement
    return -1;
}

template <typename T>
string vecToString(const vector<T>& v) {
    ostringstream out;
    out << "{";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) out << ", ";
        out << v[i];
    }
    out << "}";
    return out.str();
}

struct TestCase {
    vector<int> nums;
    long long expected;
};

int main() {
    vector<TestCase> tests = {
        {{2, 2, 3, 4}, 3},
        {{4, 2, 3, 4}, 4},
        {{0, 0, 0}, 0},
        {{1, 1, 1, 1}, 4},
        {{1, 2, 3, 4, 5, 6}, 7}
    };

    int passed = 0;
    for (size_t i = 0; i < tests.size(); ++i) {
        const auto& t = tests[i];
        long long actual = countValidTriangles(t.nums);
        bool ok = actual == t.expected;
        passed += ok ? 1 : 0;

        cout << "Test " << (i + 1) << ": " << (ok ? "PASS" : "FAIL") << "\n";
        cout << "  input:    nums=" << vecToString(t.nums) << "\n";
        cout << "  got:      " << actual << "\n";
        cout << "  expected: " << t.expected << "\n\n";
    }

    cout << "Passed " << passed << " / " << tests.size() << " tests.\n";
    return passed == static_cast<int>(tests.size()) ? 0 : 1;
}
