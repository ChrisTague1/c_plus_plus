#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "lexer.h"
#include "parser.h"

namespace {

struct ValidCase {
    std::string_view name;
    std::string_view input;
    std::string_view expected_output;
};

struct InvalidCase {
    std::string_view name;
    std::string_view input;
};

JsonValue parse(std::string_view input) {
    std::istringstream stream{std::string(input)};
    TokenStream tokens = lexer(stream);
    return parser(tokens);
}

std::string print(const JsonValue& value) {
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

bool run(const ValidCase& test) {
    try {
        JsonValue value = parse(test.input);
        const std::string actual = print(value);

        if (actual == test.expected_output) {
            std::cout << "PASS: " << test.name << '\n';
            return true;
        }

        std::cerr << "FAIL: " << test.name << '\n'
                  << "  input:    " << test.input << '\n'
                  << "  expected: " << test.expected_output << '\n'
                  << "  actual:   " << actual << '\n';
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << test.name << '\n'
                  << "  unexpected exception: " << error.what() << '\n';
    }
    return false;
}

bool run(const InvalidCase& test) {
    try {
        JsonValue value = parse(test.input);
        std::cerr << "FAIL: " << test.name << '\n'
                  << "  expected an exception, but parsed: " << print(value)
                  << '\n';
    } catch (const std::exception&) {
        std::cout << "PASS: " << test.name << '\n';
        return true;
    }
    return false;
}

}  // namespace

int main() {
    // Add successful parses here. Comparing printed output is intentionally
    // simple; object tests should contain one key because unordered_map does
    // not guarantee iteration order.
    const std::vector<ValidCase> valid_cases{
        {"null", "null", "null"},
        {"true", "true", "true"},
        {"false", "false", "false"},
        {"number", "-12.5", "-12.5"},
        {"string", R"("hello")", R"("hello")"},
        {"empty array", "[]", "[]"},
        {"mixed array", R"([true, null, 3.5, "hi"])",
         R"([true, null, 3.5, "hi"])"},
        {"empty object", "{}", "{}"},
        {"single-property object", R"({"answer": 42})",
         R"({"answer": 42})"},
        {"nested value", R"({"items": [1, 2, 3]})",
         R"({"items": [1, 2, 3]})"},
    };

    // Add malformed inputs here. Later, extend InvalidCase with an expected
    // error type/message/position when the error API becomes stable.
    const std::vector<InvalidCase> invalid_cases{
        {"unknown character", "@"},
        {"unterminated string", R"("hello)"},
        {"misspelled keyword", "truX"},
        {"object missing colon", R"({"answer" 42})"},
        {"object missing comma", R"({"a": 1 "b": 2})"},
        {"array missing comma", "[1 2]"},
        {"trailing object comma", R"({"a": 1,})"},
    };

    std::size_t passed = 0;
    const std::size_t total = valid_cases.size() + invalid_cases.size();

    for (const auto& test : valid_cases) passed += run(test);
    for (const auto& test : invalid_cases) passed += run(test);

    std::cout << "\n" << passed << '/' << total << " tests passed\n";
    return passed == total ? 0 : 1;
}
