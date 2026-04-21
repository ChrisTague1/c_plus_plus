#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <CLI11.hpp>
#include <thread>
#include <mutex>
#include <iterator>
#include <deque>
#include <condition_variable>

namespace fs = std::filesystem;

std::vector<std::string> shared_container;
std::mutex container_mutex;

void explore_directory(
    const std::string& path,
    const std::string& searchTerm
) {
    if (!fs::exists(path) || !fs::is_directory(path)) return;

    std::vector<std::string> local_results;
    std::vector<std::jthread> workers;

    for (const auto& entry : fs::directory_iterator(path)) {
        auto path = entry.path().string();

        if (entry.is_directory()) {
            workers.emplace_back([=] {
                // std::cout << "Entering dir " << path << std::endl;
                explore_directory(path, searchTerm);
            });
        }

        std::ifstream file(entry.path());

        if (!file.is_open()) {
            continue;
        }

        std::string line;

        while (std::getline(file, line)) {
            if (line.find(searchTerm) != std::string::npos) {
                local_results.push_back(line);
            }
        }
    }

    std::lock_guard<std::mutex> lock(container_mutex);
    shared_container.reserve(shared_container.size() + local_results.size()); // will this kill performance?
    shared_container.insert(
        shared_container.end(),
        std::make_move_iterator(local_results.begin()),
        std::make_move_iterator(local_results.end())
    );
}

void worker(int id, std::string& searchTerm) {

}

int main(int argc, char* argv[]) {
    CLI::App app{"Grabber"};

    std::string path = "test_data";
    std::string searchTerm = "item";

    app.add_option("-p,--path", path, "Path to directory to grab from");
    app.add_option("-s,--search", searchTerm, "Term to search for");

    CLI11_PARSE(app, argc, argv);

    explore_directory(path, searchTerm);

    for (const auto& result : shared_container) {
        std::cout << result << std::endl;
    }
    std::cout << "Found " << shared_container.size() << " total results" << std::endl;

    return 0;
}
