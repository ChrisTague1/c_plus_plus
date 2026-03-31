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
    std::cout << std::endl;
}

int main() {
    {
        std::set<std::string> programmers{"Ada", "Grace", "Margaret"};

        print_set(programmers);

        std::cout << "Adding Hedy and Linus" << std::endl;

        programmers.insert("Hedy");
        programmers.emplace("Linus");

        print_set(programmers);

        if (programmers.contains("Hedy")) {
            std::cout << "Hedy is in the list" << std::endl;
        }

        std::cout << "Removing Grace" << std::endl;

        programmers.erase("Grace");

        print_set(programmers);

        {
            if (auto [it, success] = programmers.insert("Nathan"); success) {
                std::cout << *it << " added successfully" << std::endl;;
            } else {
                std::cout << "failed to add Nathan" << std::endl;
            }
        }

        {
            if (auto [it, success] = programmers.insert("Nathan"); success) {
                std::cout << *it << " added successfully" << std::endl;;
            } else {
                std::cout << "failed to add Nathan" << std::endl;
            }
        }
    }

    std::cout << "\n\n" << std::endl;

    {
        std::map<std::string, int, std::less<>> scores{{"Ada", 100},
                                                       {"Grace", 95}};

        print_map(scores);

        scores.insert({"Margaret", 98});
        scores.emplace("Linus", 100);
        scores.try_emplace("Will", 110);
        scores["Kyle"] = 120;

        print_map(scores);

        if (scores.contains("Ada")) {
            std::cout << "Ada is in the map" << std::endl;
        }

        std::cout << "Removing Linus" << std::endl;
        scores.erase("Linus");

        print_map(scores);

        std::erase_if(scores, [](const auto& pair) {
            return pair.second < 100;
        });

        print_map(scores);

        auto node = scores.extract("Will");
        if (!node.empty()) {
            node.key() = "Will Chase"; // wow that is cursed
            scores.insert(std::move(node));
        }

        print_map(scores);

        {
            auto [it, ok] = scores.try_emplace("Nathan", 69);

            if (ok) {
                std::cout << it->first << ": " << it->second
                          << " inserted nicely" << std::endl;
            }
        }

        {
            auto [it, ok] = scores.try_emplace("Nathan", 420);

            if (ok) {
                std::cout << it->first << ": " << it->second
                          << " inserted nicely" << std::endl;
            } else {
                std::cout << it->first << ": " << it->second
                          << " not inserted nicely" << std::endl;
            }
        }
    }

    return 0;
}