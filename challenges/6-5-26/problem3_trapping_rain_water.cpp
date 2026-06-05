#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*
Problem: Trapping Rain Water

You are given a vector height where height[i] is the elevation of a vertical bar at index i.
Each bar has width 1.

After raining, water may be trapped between bars. Return the total amount of trapped water.

Constraints to assume:
- 0 <= height.size() <= 200000
- 0 <= height[i] <= 1000000000
- The answer fits in a signed 64-bit integer.
*/
long long trapRainWater(const vector<int>& height) {
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
    vector<int> height;
    long long expected;
};

int main() {
    vector<TestCase> tests = {
        {{0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6},
        {{4, 2, 0, 3, 2, 5}, 9},
        {{}, 0},
        {{1, 2, 3, 4, 5}, 0},
        {{5, 4, 3, 2, 1}, 0}
    };

    int passed = 0;
    for (size_t i = 0; i < tests.size(); ++i) {
        const auto& t = tests[i];
        long long actual = trapRainWater(t.height);
        bool ok = actual == t.expected;
        passed += ok ? 1 : 0;

        cout << "Test " << (i + 1) << ": " << (ok ? "PASS" : "FAIL") << "\n";
        cout << "  input:    height=" << vecToString(t.height) << "\n";
        cout << "  got:      " << actual << "\n";
        cout << "  expected: " << t.expected << "\n\n";
    }

    cout << "Passed " << passed << " / " << tests.size() << " tests.\n";
    return passed == static_cast<int>(tests.size()) ? 0 : 1;
}
