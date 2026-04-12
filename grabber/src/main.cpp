#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <CLI11.hpp>

namespace fs = std::filesystem;

void exploreDirectory(const std::string& path, const std::string& searchTerm) {
    if (!fs::exists(path) || !fs::is_directory(path)) return;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            exploreDirectory(entry.path().string(), searchTerm);
            continue;
        }

        std::ifstream file(entry.path());
        if (!file.is_open()) {
            continue;
        }

        std::string line;

        while (std::getline(file, line)) {
            if (line.find(searchTerm) != std::string::npos) {
                std::cout << line << std::endl;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    CLI::App app{"Grabber"};

    std::string path = "test_data";
    std::string searchTerm = "item";

    app.add_option("-p,--path", path, "Path to directory to grab from");
    app.add_option("-s,--search", searchTerm, "Term to search for");

    CLI11_PARSE(app, argc, argv);

    exploreDirectory(path, searchTerm);

    return 0;
}
