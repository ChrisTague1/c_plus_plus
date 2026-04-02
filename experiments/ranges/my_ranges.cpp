#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>

void print(auto&& r, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& x : r) std::cout << x << " ";
    std::cout << std::endl;
}

int main() {
    {
        std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};

        std::ranges::sort(v);
        print(v, "sorted");

        std::ranges::sort(v, std::greater{});
        print(v, "descending");

        auto it = std::ranges::find(v, 7);
        if (it != v.end())
            std::cout << "found: " << *it << std::endl;

        std::cout << "any > 8: " << std::ranges::any_of(v, [](int x) { return x > 8; }) << std::endl;
        std::cout << "all > 0: " << std::ranges::all_of(v, [](int x) { return x > 0; }) << std::endl;
        std::cout << "none < 0: " << std::ranges::none_of(v, [](int x) { return x < 0; }) << std::endl;

        auto evens = std::ranges::count_if(v, [](int x) { return x % 2 == 0; });
        std::cout << "even count: " << evens << std::endl;

        std::ranges::reverse(v);
        print(v, "reversed");

        std::cout << "min: " << std::ranges::min(v) << std::endl;
        std::cout << "max: " << std::ranges::max(v) << std::endl;
    }

    return 0;
}