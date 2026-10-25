#include <iostream>
#include <string_view>
#include <vector>

void printer(const std::string_view& str) {
    std::cout << "  ";
    for (int i = 0; i < str.size(); ++i) {
        std::cout << str[i];

        if (i != str.size() - 1) std::cout << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < str.size(); ++i) {
        std::cout << str[i] << " ";

        for (int j = 0; j < str.size(); ++j) {
            if (i == j || str[i] != str[j]) {
                std::cout << "  ";
            } else if (str[i] == str[j]) {
                std::cout << "X ";
            }
        }
        std::cout << "\n\n";
    }

    std::cout << "\n\n";
}

bool repeatable(const std::string_view& s) {
    // Did not use this solution at all: https://leetcode.com/problems/repeated-substring-pattern/
    std::vector<int> logs(s.size());

    for (int i = 0; i < s.size(); ++i) {
        for (int j = s.size() - 1; j >= 0; --j) {
            if (i == j) continue;

            logs[j] = s[i] == s[j];

            if (logs[j] && j > 0) {
                logs[j] += logs[j - 1];
            }
        }
    }
}

struct TestCase {
    std::string_view in;
    bool expected;
};

int main() {
    using namespace std;

    // printer("abcabcdabcabcd");
    printer("abcabcabcabc");

    vector<TestCase> cases = {
        {
            "abab",
            true
        },
        {
            "aba",
            false
        },
        {
            "abcabcabcabc",
            true
        },
        {
            "abcabcdabcabcd",
            true
        }
    };

    vector<int> wrong;

    for (int i = 0; i < cases.size(); ++i) {
        const auto& test = cases[i];

        auto actual = repeatable(test.in);

        if (actual != test.expected) {
            wrong.push_back(i);
        }
    }

    for (auto& w : wrong) {
        const auto& test = cases[w];

        std::cout << w << ". " << test.in << " is " << (test.expected ? "true" : "false") << "\n";
    }
    int correct = cases.size() - wrong.size();
    std::cout << correct << "/" << cases.size() << "\n";

    return 0;
}