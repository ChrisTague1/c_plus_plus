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
        // ── Single character (no answer) ──
        {"a",          "no answer"},
        {"z",          "no answer"},
        {"m",          "no answer"},

        // ── Two characters ──
        {"ab",         "ba"},
        {"ba",         "no answer"},
        {"bb",         "no answer"},
        {"az",         "za"},
        {"za",         "no answer"},
        {"mn",         "nm"},

        // ── Already max permutation (fully descending) ──
        {"dcba",       "no answer"},
        {"zz",         "no answer"},
        {"zyx",        "no answer"},
        {"zzzzz",      "no answer"},
        {"cba",        "no answer"},
        {"edcba",      "no answer"},
        {"rqponm",     "no answer"},

        // ── Already min permutation (fully ascending) ──
        {"abc",        "acb"},
        {"abcd",       "abdc"},
        {"abcde",      "abced"},
        {"abcdef",     "abcdfe"},
        {"lmno",       "lmon"},

        // ── Swap only the last two ──
        {"abdc",       "acbd"},
        {"hegf",       "hfeg"},
        {"dcab",       "dcba"},
        {"zyab",       "zyba"},

        // ── Pivot is deep in the string ──
        {"dkhc",       "hcdk"},
        {"abfedc",     "acbdef"},
        {"abcfed",     "abdcef"},
        {"xyzedcba",   "xzabcdey"},

        // ── All same characters (no answer) ──
        {"aaa",        "no answer"},
        {"bbbbb",      "no answer"},
        {"cccccc",     "no answer"},

        // ── Repeated characters mixed ──
        {"aab",        "aba"},
        {"aba",        "baa"},
        {"baa",        "no answer"},
        {"aabb",       "abab"},
        {"abab",       "abba"},
        {"abba",       "baab"},
        {"baab",       "baba"},
        {"baba",       "bbaa"},
        {"bbaa",       "no answer"},
        {"aaab",       "aaba"},
        {"aaba",       "abaa"},
        {"abaa",       "baaa"},
        {"baaa",       "no answer"},
        {"aabbc",      "aabcb"},
        {"abcba",      "acabb"},

        // ── Full permutation chains (verify every step) ──
        // all permutations of "abc": abc→acb→bac→bca→cab→cba
        {"acb",        "bac"},
        {"bac",        "bca"},
        {"bca",        "cab"},
        {"cab",        "cba"},
        {"cba",        "no answer"},

        // all permutations of "112": 112→121→211
        {"112",        "121"},
        {"121",        "211"},
        {"211",        "no answer"},

        // ── Longer strings ──
        {"hefg",       "hegf"},
        {"dhck",       "dhkc"},
        {"zzdkhhc",    "zzhcdhk"},
        {"fedcbabcd",  "fedcbabdc"},
        {"abcdefgh",   "abcdefhg"},
        {"hgfedcba",   "no answer"},
        {"imhacbfed",  "imhacdbef"},

        // ── Strings with wide character gaps ──
        {"azcba",      "baacz"},
        {"amnzyx",     "amxnyz"},

        // ── Near-end pivots ──
        {"abcedcba",   "abdabcce"},
        {"xaaa",       "no answer"},

        // ── Ascending then descending ──
        {"abcdzyx",    "abcxdyz"},
        {"mnopzyx",    "mnoxpyz"},

        // ── Two-char alphabet longer strings ──
        {"aabba",      "abaab"},
        {"baaab",      "baaba"},
        {"aaabb",      "aabab"},
        {"bbaba",      "bbbaa"},
        {"abbba",      "baabb"},
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