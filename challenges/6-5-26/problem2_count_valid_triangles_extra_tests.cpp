#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Paste your implementation here, or compile/link this file with an implementation file.
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
        {{}, 0},
        {{1}, 0},
        {{1, 1}, 0},
        {{1, 1, 2}, 0},
        {{1, 1, 1}, 1},
        {{2, 3, 4, 5, 6}, 7},
        {{10, 21, 22, 100, 101, 200, 300}, 6},
        {{3, 3, 3, 3, 3}, 10},
        {{0, 1, 1, 1, 2, 2, 3}, 7},
        {{5, 10, 12, 13, 15, 18}, 17}
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
