#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

vector<string> the_algo(const vector<string>& dict, const string& input) {
    unordered_set<char> allChars;

    for (const auto& item : dict) {
        allChars.insert(item.begin(), item.end());
        cout << item << " ";
    }
    cout << endl;

    cout << input << endl;

    for (const auto& ch : allChars) {
        cout << ch << " ";
    }
    cout << endl;

    return {};
}

struct TestCase {
    vector<string> dict;
    vector<string> output;
    string input;
};

int main() {
    vector<TestCase> testCases = {
        {
            {"a", "ab", "bab", "bc", "bca", "c", "caa"},
            {"a", "ab", "bc", "c", "c", "a", "ab"},
            "abccab"
        }
    };

    for (const auto& testCase : testCases) {
        auto result = the_algo(testCase.dict, testCase.input);

        if (result != testCase.output) {
            cout << "Failed!" << endl;
        } else {
            cout << "Passed!" << endl;
        }
    }

    return 0;
}