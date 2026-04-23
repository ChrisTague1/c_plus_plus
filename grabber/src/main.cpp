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
#include <vector>

namespace fs = std::filesystem;

std::deque<std::string> dq;
std::vector<std::string> results;
std::mutex mtx;
std::condition_variable_any cv; // TODO is an atomic or something to track when it is done

void worker(std::stop_token st, int id, std::string searchTerm) {
    std::unique_lock lock(mtx);

    while (true) {
        cv.wait(lock, st, []() {
            return !dq.empty();
        });

        if (st.stop_requested() && dq.empty()) {
            std::cout << "Worker " << id << " stopping\n";
            break;
        }

        while (!dq.empty()) {
            std::string path = std::move(dq.back());
            dq.pop_back();

            bool should_notify = !dq.empty();

            lock.unlock();

            if (should_notify) {
                cv.notify_all();
            }

            std::vector<std::string> local_results;
            std::vector<std::string> further_dirs;

            for (const auto& entry : fs::directory_iterator(path)) {
                auto path = entry.path().string();

                if (entry.is_directory()) {
                    further_dirs.emplace_back(std::move(path));
                    continue;
                }

                std::ifstream file(path);

                if (!file.is_open()) continue;

                std::string line;

                while (std::getline(file, line)) {
                    if (line.find(searchTerm) != std::string::npos) {
                        local_results.emplace_back(std::move(line));
                    }
                }
            }

            lock.lock();

            results.reserve(results.size() + local_results.size());
            results.insert(
                results.end(),
                std::make_move_iterator(local_results.begin()),
                std::make_move_iterator(local_results.end())
            );

            for (auto& dir : further_dirs) {
                dq.emplace_back(std::move(dir));
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

    if (!fs::exists(path) || !fs::is_directory(path)) {
        std::cerr << "You gotta provide a valid path man\n";
        return -1;
    }

    std::vector<std::jthread> workers;
    workers.reserve(10);

    for (int i = 0; i < 10; i++) {
        workers.emplace_back(worker, i, searchTerm);
    }

    {
        std::lock_guard lock(mtx);
        dq.emplace_back(path);
    }
    cv.notify_one();

    return 0;
}
