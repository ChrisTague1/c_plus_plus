#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

int main() {
    std::string path = "test_data";

    try {
        if (fs::exists(path) && fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                std::cout << entry.path().filename() << std::endl;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::ifstream file("README.md");

    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }

        file.close();
    } else {
        std::cerr << "Unable to open file README.md" << std::endl;
    }

    std::ifstream file2("README.md");

    std::stringstream buffer;
    buffer << file2.rdbuf();
    std::string content = buffer.str();

    std::cout << content << std::endl;

    return 0;
}
