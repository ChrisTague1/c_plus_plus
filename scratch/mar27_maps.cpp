#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

int main() {
    std::map<int, std::string> sortedStudents;

    sortedStudents[30] = "Charlie";
    sortedStudents[10] = "Alice";
    sortedStudents[20] = "Bob";

    std::cout << "Map" << std::endl;
    for (const auto& [id, name] : sortedStudents) {
        std::cout << "\t" << id << ": " << name << std::endl;
    }

    std::unordered_map<int, std::string> fastStudents;
    fastStudents.reserve(3);
    fastStudents.try_emplace(30, "Charlie");
    fastStudents.try_emplace(10, "Alice");
    fastStudents.try_emplace(20, "Bob");

    std::cout << "Unordered map" << std::endl;
    for (const auto& [id, name] : fastStudents) {
        std::cout << "\t" << id << ": " << name << std::endl;
    }

    {
        auto [it, inserted] = fastStudents.try_emplace(40, "Chris");

        if (inserted) {
            std::cout << "It inserted!" << std::endl;
            std::cout << it->first << std::endl;
            std::cout << it->second << std::endl;
        } else {
            std::cout << "It did not insert" << std::endl;
            std::cout << it->first << std::endl;
            std::cout << it->second << std::endl;
        }
    }

    {
        auto [it, inserted] = fastStudents.try_emplace(40, "Chris");

        if (inserted) {
            std::cout << "It inserted!" << std::endl;
            std::cout << it->first << std::endl;
            std::cout << it->second << std::endl;
        } else {
            std::cout << "It did not insert" << std::endl;
            std::cout << it->first << std::endl;
            std::cout << it->second << std::endl;
        }
    }

    return 0;
}
