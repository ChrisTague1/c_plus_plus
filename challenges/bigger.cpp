#include <iostream>
#include <queue>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>

using std::cout;
using std::endl;

std::string biggerIsGreater2(std::string w) {
    const int n = static_cast<int>(w.size());
    if (n <= 1) return "no answer";

    int i = n - 2;
    while (i >= 0 && w[i] >= w[i + 1])
        --i;

    if (i < 0) return "no answer";

    int j = n - 1;
    while (w[j] <= w[i])
        --j;

    std::swap(w[i], w[j]);

    std::reverse(w.begin() + i + 1, w.end());

    return w;
}

std::string biggerIsGreater(const std::string& w) {
    std::priority_queue<char> heap;

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

    char pivot = w[index];

    std::string temp;

    found = false;

    while (!heap.empty()) {
        char top = heap.top();
        heap.pop();

        if (!found && top == pivot) {
            ss << temp.back();
            temp.pop_back();
            found = true;
        }

        temp.push_back(top);
    }

    std::reverse(temp.begin(), temp.end());

    ss << temp;

    return ss.str();
}


struct TestCase {
    std::string input;
    std::string expected;
};

std::string biggerIsGreater3(const std::string& a) { // Chris' version of the good one
    std::string w(a);

    if (w.size() <= 1) {
        return "no answer";
    }

    int i;
    for (i = w.size() - 2; i != -1; i--) {
        if (w[i] < w[i + 1]) {
            break;
        }
    }

    if (i == -1) return "no answer";

    char pivot = w[i];

    for (int j = w.size() - 1; j >= 0; j--) {
        if (w[j] > pivot) {
            w[i] = w[j];
            w[j] = pivot;
            break;
        }
    }

    std::reverse(w.begin() + i + 1, w.end());

    return w;
}

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

    auto run = [&](const char* label, auto fn) {
        int p = 0, f = 0;
        cout << "\n── " << label << " ──\n";
        for (const auto& t : tests) {
            std::string result = fn(t.input);
            bool ok = (result == t.expected);
            ok ? ++p : ++f;
            if (ok) {
                cout << "  ✅ PASS";
            } else {
                cout << "  ❌ FAIL";
            }
            cout << "  \"" << t.input << "\" → \"" << result << "\"";
            if (!ok) cout << "  (expected \"" << t.expected << "\")";
            cout << "\n";
        }
        cout << "── " << label << ": " << p << " passed, " << f << " failed, "
             << tests.size() << " total ──\n";
        passed += p; failed += f;
    };

    run("biggerIsGreater  (original)", [](const std::string& s) { return biggerIsGreater(s); });
    run("biggerIsGreater2 (improved)", [](const std::string& s) { return biggerIsGreater2(s); });
    run("biggerIsGreater3 (improved)", [](const std::string& s) { return biggerIsGreater3(s); });

    cout << "\n══ TOTAL: " << passed << " passed, " << failed << " failed ══\n";
}

int main() {
    runTests();
    return 0;
}