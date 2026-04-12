#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

    // std::ifstream file2("README.md");

    // std::stringstream buffer;
    // buffer << file2.rdbuf();
    // std::string content = buffer.str();

    // std::cout << content << std::endl;

void exploreDirectory(const std::string& path) {
    if (!fs::exists(path) || !fs::is_directory(path)) return;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            exploreDirectory(entry.path().relative_path());
            continue;
        }

        std::ifstream file(entry.path());
        std::stringstream ss;
        ss << file.rdbuf();
        std::string content = ss.str();

        std::cout << content << std::endl;
    }
}

int main() {
    std::string path = "test_data";

    exploreDirectory(path);

    // try {
    //     if (fs::exists(path) && fs::is_directory(path)) {
    //         for (const auto& entry : fs::directory_iterator(path)) {
    //             bool isRegFile = entry.is_regular_file();
    //             bool isDir = entry.is_directory();
    //             std::cout << entry.path().filename() << ", is file = " << isRegFile << ", is dir = " << isDir << std::endl;
    //         }
    //     }
    // } catch (const fs::filesystem_error& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    // }

    // std::ifstream file("README.md");

    // if (file.is_open()) {
    //     std::string line;

    //     while (std::getline(file, line)) {
    //         std::cout << line << std::endl;
    //     }

    //     file.close();
    // } else {
    //     std::cerr << "Unable to open file README.md" << std::endl;
    // }

    // std::ifstream file2("README.md");

    // std::stringstream buffer;
    // buffer << file2.rdbuf();
    // std::string content = buffer.str();

    // std::cout << content << std::endl;

    return 0;
}
