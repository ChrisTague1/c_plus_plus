#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Paste your implementation here, or compile/link this file with an implementation file.
long long minimumEatingSpeed(const vector<int>& piles, long long h) {
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
    vector<int> piles;
    long long h;
    long long expected;
};

int main() {
    vector<TestCase> tests = {
        {{312884470}, 312884469, 2},
        {{5, 5, 5, 5}, 4, 5},
        {{5, 5, 5, 5}, 8, 3},
        {{9, 9, 9}, 27, 1},
        {{8, 16, 24, 32}, 10, 8},
        {{805306368, 805306368, 805306368}, 1000000000LL, 3},
        {{1, 2, 3, 4, 5}, 15, 1},
        {{1, 2, 3, 4, 5}, 5, 5}
    };

    int passed = 0;
    for (size_t i = 0; i < tests.size(); ++i) {
        const auto& t = tests[i];
        long long actual = minimumEatingSpeed(t.piles, t.h);
        bool ok = actual == t.expected;
        passed += ok ? 1 : 0;

        cout << "Test " << (i + 1) << ": " << (ok ? "PASS" : "FAIL") << "\n";
        cout << "  input:    piles=" << vecToString(t.piles) << ", h=" << t.h << "\n";
        cout << "  got:      " << actual << "\n";
        cout << "  expected: " << t.expected << "\n\n";
    }

    cout << "Passed " << passed << " / " << tests.size() << " tests.\n";
    return passed == static_cast<int>(tests.size()) ? 0 : 1;
}
