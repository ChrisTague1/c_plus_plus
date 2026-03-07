#include <cassert>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string_view>
#include <system_error>

int main() {
    auto data = {
        "1234",
        "15 foo",
        "bar",
        " 42",
        "5000000000000"
    };
    for (std::string_view const str : data) {
        std::cout << "String: " << std::quoted(str) << ". ";
        int result;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

        if (ec == std::errc()) {
            std::cout << "Result: " << result << ", ptr -> " << std::quoted(ptr) << "\n";
        } else if (ec == std::errc::invalid_argument) { 
            std::cout << "This is not a number.\n";
        } else if (ec == std::errc::result_out_of_range) {
            std::cout << "This number is larger than an int.\n";
        }
    }
}