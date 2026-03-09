#include "extract_num.h"
#include <charconv>

int extract_num(std::string_view str) {
    int value;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

    if (ec == std::errc()) {
        return value;
    }

    return 0;
}
