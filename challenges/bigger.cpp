#include <iostream>
#include <queue>
#include <string>
#include <iterator>
#include <sstream>

using std::cout;
using std::endl;

std::string biggerIsGreater(const std::string& w) {
    std::priority_queue<char> heap; // could use one-hot encoding a 32 bit number to do this instead

    bool found = false;
    int index = -1;

    for (auto rit = w.rbegin(); rit != w.rend(); rit++) {
        heap.push(*rit);

        if (*rit < heap.top()) {
            found = true;
            index = w.size() - 1 - std::distance(w.rbegin(), rit);
            break;
        }
    }

    if (!found) return "no answer";

    std::stringstream ss;

    ss << w.substr(0, index);

    int i;

    for (i = w.size() - 1; i >= 0; i--) {
        if (w[i] > w[index]) {
            break;
        }
    }

    ss << w.substr(i, w.size() - 1) << w[index];

    for (i = i - 1; i > index; i--) {
        ss << w[i];
    }

    return ss.str();
}

struct TestCase {
    std::string input;
    std::string expected;
};

void runTests() {
    std::vector<TestCase> tests = {
        // Basic cases
        {"ab",       "ba"},
        {"bb",       "no answer"},
        {"hefg",     "hegf"},
        {"dhck",     "dhkc"},
        {"dkhc",     "hcdk"},

        // Single char
        {"a",        "no answer"},

        // Already max permutation (descending)
        {"dcba",     "no answer"},
        {"zz",       "no answer"},

        // Already min permutation (ascending)
        {"abcd",     "abdc"},
        {"abdc",     "acbd"},

        // Repeated characters
        {"aab",      "aba"},
        {"aba",      "baa"},

        // Longer strings
        {"zzdkhhc",  "zzhcdhk"},
        {"lmno",     "lmon"},
        {"fedcbabcd", "fedcbabdc"},
    };

    int passed = 0;
    int failed = 0;

    for (const auto& t : tests) {
        std::string result = biggerIsGreater(t.input);
        bool ok = (result == t.expected);

        if (ok) {
            cout << "  ✅ PASS";
            passed++;
        } else {
            cout << "  ❌ FAIL";
            failed++;
        }

        cout << "  \"" << t.input << "\" → \"" << result << "\"";
        if (!ok) {
            cout << "  (expected \"" << t.expected << "\")";
        }
        cout << "\n";
    }

    cout << "\n── Results: " << passed << " passed, " << failed << " failed, "
         << tests.size() << " total ──\n";
}

int main() {
    runTests();
    return 0;
}