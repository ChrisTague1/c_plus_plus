#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

void exploreDirectory(const std::string& path, const std::string& searchTerm) {
    if (!fs::exists(path) || !fs::is_directory(path)) return;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            exploreDirectory(entry.path().relative_path(), searchTerm);
            continue;
        }

        std::ifstream file(entry.path());
        std::stringstream ss;
        ss << file.rdbuf();
        std::string content = ss.str();

        if (content.find(searchTerm) != std::string::npos) {
            std::cout << content << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    std::string path = "test_data";
    std::string searchTerm = "item";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-p" || arg == "--path") {
            if (i + 1 < argc) {
                path = argv[++i];
            } else {
                std::cerr << "--path requires a directory\n";
                return 1;
            }
        }

        if (arg == "-s" || arg == "--search") {
            if (i + 1 < argc) {
                searchTerm = argv[++i];
            } else {
                std::cerr << "--search requires a search term\n";
                return 1;
            }
        }
    }

    exploreDirectory(path, searchTerm);

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
