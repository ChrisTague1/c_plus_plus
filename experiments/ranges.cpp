#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>

// ============================================================
// C++20 RANGES
// ============================================================
// The ranges library gives you:
//   - std::ranges::* algorithms  — take containers directly, no begin/end
//   - Views                      — lazy, composable transformations
//   - Pipe syntax                — chain views with |
// ============================================================

void print(auto&& r, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& x : r) std::cout << x << " ";
    std::cout << "\n";
}


// ---- 1. RANGES ALGORITHMS ----------------------------------
// std::ranges::* versions accept containers directly — no more begin/end

void range_algorithms() {
    std::cout << "\n--- 1. Ranges algorithms ---\n";

    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // sort — takes the container, not iterators
    std::ranges::sort(v);
    print(v, "sorted");

    // sort with a comparator
    std::ranges::sort(v, std::greater{});
    print(v, "descending");

    // find
    auto it = std::ranges::find(v, 7);
    if (it != v.end())
        std::cout << "found: " << *it << "\n";

    // any_of / all_of / none_of
    std::cout << "any > 8: "  << std::ranges::any_of(v,  [](int x) { return x > 8; }) << "\n";
    std::cout << "all > 0: "  << std::ranges::all_of(v,  [](int x) { return x > 0; }) << "\n";
    std::cout << "none < 0: " << std::ranges::none_of(v, [](int x) { return x < 0; }) << "\n";

    // count_if
    auto evens = std::ranges::count_if(v, [](int x) { return x % 2 == 0; });
    std::cout << "even count: " << evens << "\n";

    // reverse
    std::ranges::reverse(v);
    print(v, "reversed");

    // min / max
    std::cout << "min: " << std::ranges::min(v) << "\n";
    std::cout << "max: " << std::ranges::max(v) << "\n";
}


// ---- 2. PROJECTIONS ----------------------------------------
// Ranges algorithms accept a projection — a function applied to each
// element before comparing/searching. Avoids writing custom comparators.

struct Player {
    std::string name;
    int score;
};

void projections() {
    std::cout << "\n--- 2. Projections ---\n";

    std::vector<Player> players = {
        {"Alice", 42},
        {"Bob",   17},
        {"Carol", 98},
        {"Dave",  55},
    };

    // sort by score — projection extracts the field to sort on
    std::ranges::sort(players, std::less{}, &Player::score);
    for (auto& p : players)
        std::cout << p.name << ": " << p.score << "\n";

    // find by name
    auto it = std::ranges::find(players, "Carol", &Player::name);
    if (it != players.end())
        std::cout << "found: " << it->name << " with score " << it->score << "\n";

    // max by score
    auto top = std::ranges::max(players, std::less{}, &Player::score);
    std::cout << "top scorer: " << top.name << "\n";
}


// ---- 3. VIEWS — LAZY TRANSFORMATIONS -----------------------
// Views don't modify or copy data — they describe a transformation
// that is applied element-by-element as you iterate.

void views_basics() {
    std::cout << "\n--- 3. Views basics ---\n";

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // filter — only elements matching the predicate
    auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });
    print(evens, "evens");

    // transform — apply a function to each element
    auto squared = v | std::views::transform([](int x) { return x * x; });
    print(squared, "squared");

    // take — first N elements
    auto first5 = v | std::views::take(5);
    print(first5, "first 5");

    // drop — skip first N elements
    auto after3 = v | std::views::drop(3);
    print(after3, "drop 3");

    // reverse
    auto rev = v | std::views::reverse;
    print(rev, "reversed");
}


// ---- 4. PIPE CHAINING --------------------------------------
// Views compose with | — each step is still lazy, no intermediate vectors

void pipe_chaining() {
    std::cout << "\n--- 4. Pipe chaining ---\n";

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // evens, squared, first 3 — all in one pass, no allocations
    auto result = v
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3);

    print(result, "even squares, first 3");

    // works on strings too
    std::vector<std::string> words = {"apple", "banana", "avocado", "cherry", "apricot"};
    auto a_words = words
        | std::views::filter([](const std::string& s) { return s.starts_with('a'); })
        | std::views::transform([](const std::string& s) { return s.size(); });

    print(a_words, "lengths of 'a' words");
}


// ---- 5. RANGE FACTORIES ------------------------------------
// Views that generate ranges from scratch, not from a container

void range_factories() {
    std::cout << "\n--- 5. Range factories ---\n";

    // iota — generates [start, end)
    auto nums = std::views::iota(1, 11);
    print(nums, "iota 1-10");

    // iota unbounded + take
    auto first10odds = std::views::iota(1)
        | std::views::filter([](int x) { return x % 2 != 0; })
        | std::views::take(10);
    print(first10odds, "first 10 odds");

    // iota as a replacement for index loops
    std::vector<std::string> names = {"Alice", "Bob", "Carol"};
    for (auto i : std::views::iota(0u, names.size()))
        std::cout << i << ": " << names[i] << "\n";
}


// ---- 6. VIEWS::ENUMERATE / ZIP (C++23 preview) -------------
// enumerate is C++23, but zip and others are worth knowing about

void zip_and_friends() {
    std::cout << "\n--- 6. zip ---\n";

    // zip — iterate two ranges in lockstep  (C++23, works in clang with c++2b)
    // Uncomment if your toolchain supports it:
    //
    // std::vector<std::string> names = {"Alice", "Bob", "Carol"};
    // std::vector<int> scores = {42, 17, 98};
    // for (auto [name, score] : std::views::zip(names, scores))
    //     std::cout << name << ": " << score << "\n";

    // C++20 workaround using iota + index
    std::vector<std::string> names = {"Alice", "Bob", "Carol"};
    std::vector<int> scores = {42, 17, 98};
    for (auto i : std::views::iota(0u, names.size()))
        std::cout << names[i] << ": " << scores[i] << "\n";
}


// ---- 7. MATERIALIZING A VIEW INTO A VECTOR -----------------
// Views are lazy — to get a concrete vector, you must copy them out.
// C++23 adds std::ranges::to<>; in C++20 use the workaround below.

void materializing() {
    std::cout << "\n--- 7. Materializing views ---\n";

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto view = v
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; });

    // C++20 workaround: construct vector from the view's iterators
    std::vector<int> result(view.begin(), view.end());
    print(result, "materialized");

    // Or use ranges::copy into a back_inserter
    std::vector<int> result2;
    std::ranges::copy(view, std::back_inserter(result2));
    print(result2, "via copy");
}


// ---- 8. VIEWS ON NON-VECTOR CONTAINERS ---------------------
// Ranges work on any range — list, string, raw arrays, etc.

void non_vector() {
    std::cout << "\n--- 8. Non-vector containers ---\n";

    // works on std::list (no random access needed)
    std::list<int> lst = {3, 1, 4, 1, 5, 9, 2, 6};
    auto big = lst | std::views::filter([](int x) { return x > 3; });
    print(big, "list elements > 3");

    // works on raw arrays
    int arr[] = {10, 20, 30, 40, 50};
    auto doubled = arr | std::views::transform([](int x) { return x * 2; });
    print(doubled, "doubled array");

    // works on strings (range of chars)
    std::string s = "Hello, World!";
    auto upper_chars = s | std::views::filter([](char c) { return std::isupper(c); });
    print(upper_chars, "uppercase chars");
}


int main() {
    range_algorithms();
    projections();
    views_basics();
    pipe_chaining();
    range_factories();
    zip_and_friends();
    materializing();
    non_vector();

    return 0;
}
