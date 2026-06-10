#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*
Problem: Minimum Eating Speed

You are given a vector piles where piles[i] is the number of items in pile i.
You are also given an integer h representing the maximum number of hours available.

Each hour, you may choose exactly one non-empty pile and eat up to speed items from it.
If a pile has fewer than speed items remaining, you finish that pile during the hour.

Return the minimum positive integer speed such that all piles can be finished within h hours.

Constraints to assume:
- 1 <= piles.size() <= 100000
- 1 <= piles[i] <= 1000000000
- piles.size() <= h <= 1000000000
*/
// TODO if you make left and right point at valid values you can just return right without tracking best
/*
- Write a massive test case generator
- benchmark a worse solution
- benchmark your own
- optimize
*/
long long minimumEatingSpeed(const vector<int>& piles, long long h) {
    int max = 0;
    int size = static_cast<int>(piles.size());

    for (int i = 0; i < size; ++i) {
        if (piles[i] > max) {
            max = piles[i];
        }
    }

    int best = max;

    long long left = 0;
    long long right = max + 1;

    while (right - left > 1) {
        int mid = left + (right - left) / 2;
        int total = 0;

        for (int i = 0; i < size; ++i) {
            int a = piles[i];
            int count = a / mid + (a % mid != 0);
            total += count;

            if (total > h) break;
        }

        if (total <= h && mid < best) {
            best = mid;
            right = mid;
        } else {
            left = mid;
        }
    }

    return best;
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
        {{3, 6, 7, 11}, 8, 4},
        {{30, 11, 23, 4, 20}, 5, 30},
        {{30, 11, 23, 4, 20}, 6, 23},
        {{1}, 1, 1},
        {{1000000000}, 2, 500000000LL}
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
