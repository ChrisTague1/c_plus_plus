// CHALLENGE: Character Frequency
//
// Given a string, return a map where each key is a character that appears
// in the string and each value is how many times it appears. Only count
// alphabetic characters (use isalpha()), and treat uppercase as lowercase
// (use tolower()).
//
// Example: "Hello" -> {{'e',1}, {'h',1}, {'l',2}, {'o',1}}
//
// Hint: Use map's operator[] — accessing a key that doesn't exist yet
// automatically initializes its value to 0 for ints. Iterate the string
// with a range-based for loop.

#include <iostream>
#include <string>
#include <map>

using namespace std;

map<char, int> charFrequency(const string& s) {
    map<char, int> frequencyMap;

    for (const auto& ch : s) {
        if (!isalpha(ch)) continue;

        frequencyMap[tolower(ch)] += 1;
    }

    return frequencyMap;
}

bool test(string input, map<char, int> expected, int testNum) {
    map<char, int> result = charFrequency(input);
    bool pass = (result == expected);
    cout << "Test " << testNum << ": " << (pass ? "PASS" : "FAIL") << endl;
    if (!pass) {
        cout << "  Expected: ";
        for (auto& [k, v] : expected) cout << k << ":" << v << " ";
        cout << "\n  Got:      ";
        for (auto& [k, v] : result) cout << k << ":" << v << " ";
        cout << endl;
    }
    return pass;
}

int main() {
    int passed = 0, total = 5;

    passed += test("Hello", {{'e',1},{'h',1},{'l',2},{'o',1}}, 1);
    passed += test("aaa", {{'a',3}}, 2);
    passed += test("", {}, 3);
    passed += test("A b C", {{'a',1},{'b',1},{'c',1}}, 4);
    passed += test("112!!x", {{'x',1}}, 5);

    cout << "\n" << passed << "/" << total << " tests passed." << endl;
    return passed == total ? 0 : 1;
}
