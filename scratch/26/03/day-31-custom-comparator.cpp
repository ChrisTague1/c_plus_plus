#include <iostream>
#include <string>
#include <set>
#include <map>
#include <algorithm>

template <typename T>
void print_set(std::set<T> theSet) {
    for (const auto& item : theSet) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

template <typename K, typename V, typename Comp>
void print_map(std::map<K, V, Comp> theMap) {
    for (const auto& [key, value] : theMap) {
        std::cout << key << ": " << value << std::endl;
    }
}

struct CaseInsensitiveLess {
    using is_transparent = void;

    template <typename T, typename U>
    bool operator()(const T& lhs, const U& rhs) const {
        std::string_view sv1 = lhs;
        std::string_view sv2 = rhs;

        return std::lexicographical_compare(
            sv1.begin(), sv1.end(),
            sv2.begin(), sv2.end(),
            [](char a, char b) {
                return std::tolower(a) < std::tolower(b);
            }
        );
    }
};

int main() {
    std::set<std::string> programmers {"Ada", "Grace", "Margaret"};

    std::map<std::string, int, std::less<>> scores {
        {"Ada", 100},
        {"Grace", 95}
    };

    print_set(programmers);

    std::cout << std::endl;

    print_map(scores);

    std::map<std::string, int, CaseInsensitiveLess> scores2;
    scores2["Alice"] = 100;

    if (scores2.contains("ALICE")) {
        std::cout << "Found ALICE in scores2" << std::endl;
    }

    return 0;
}