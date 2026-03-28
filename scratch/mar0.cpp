#include <iostream>
#include <string>

int main() {
    std::string a = "22295058132229505813";

    auto ind = a.find("9505", 3);

    if (ind == std::string::npos) {
        std::cout << "Find did not find\n";
    }

    std::cout << "Find said " << ind << "\n";

    return 0;
}