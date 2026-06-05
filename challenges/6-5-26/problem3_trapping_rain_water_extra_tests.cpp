#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Paste your implementation here, or compile/link this file with an implementation file.
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
        {{2, 0, 2}, 2},
        {{3, 0, 0, 2, 0, 4}, 10},
        {{0, 0, 0, 0}, 0},
        {{1}, 0},
        {{1, 0}, 0},
        {{1, 0, 1, 0, 1}, 2},
        {{9, 6, 8, 8, 5, 6, 3}, 3},
        {{1000000000, 0, 1000000000}, 1000000000LL},
        {{5, 0, 5, 0, 5}, 10},
        {{0, 7, 1, 4, 6}, 7}
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
