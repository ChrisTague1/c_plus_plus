#include <iostream>
#include <vector>
#include <string>

bool is_long_name(const std::string_view& name, const std::string_view& typed) {
    int n = -1;
    int t = -1;

    while (++n < name.size() && ++t < typed.size()) {
        char ch = name[n];
        int nc = 1;

        while (n + 1 < name.size() && name[n + 1] == ch) {
            ++n;
            ++nc;
        }

        if (typed[t] != ch) return false;

        int tc = 1;

        while (t + 1 < typed.size() && typed[t + 1] == ch) {
            ++t;
            tc++;
        }

        if (tc < nc) return false;

        if (n + 1 == name.size() && t + 1 == typed.size()) return true;
    }

    return false;
}

struct Test {
    std::string_view name;
    std::string_view typed;
    bool answer;
};

int main() {
    std::vector<Test> tests = {
        {
            .name = "alex",
            .typed = "aaleex",
            .answer = true
        },
        {
            .name = "saeed",
            .typed = "ssaaedd",
            .answer = false
        },
    };

    for (const auto& test : tests) {
        bool actual = is_long_name(test.name, test.typed);

        if (actual != test.answer) {
            std::cout << "You are retarded!\n";
            std::cout << "\tname: " << test.name << "\n\ttyped: " << test.typed << "\n\tanswer: " << (test.answer ? "true" : "false") << "\n";
            break;
        }
    }

    std::cout << "You did it!\n";

    return 0;
}