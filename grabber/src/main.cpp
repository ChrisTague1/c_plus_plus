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
int active_workers = 0;

void worker(std::stop_token st, int id, std::string searchTerm) {
    std::cout << "Thread " << id << " started, no lock yet\n";
    std::unique_lock lock(mtx);
    std::cout << "Thread " << id << " got the lock\n";

    while (true) {
        cv.wait(lock, st, []() {
            return !dq.empty();
        });

        if (st.stop_requested() && dq.empty() && !active_workers) {
            std::cout << "Worker " << id << " stopping\n";
            break;
        }

        while (!dq.empty()) {
            ++active_workers;
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
            --active_workers;
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

    int NUM_THREADS = 2;

    std::vector<std::jthread> workers;
    workers.reserve(NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        workers.emplace_back(worker, i, searchTerm);
    }

    std::cout << "Main thread done creating threads\n";

    {
        std::lock_guard lock(mtx);
        std::cout << "Main thread got the lock\n";
        dq.emplace_back(path);
    }
    std::cout << "Main thread no longer has the lock\n";
    cv.notify_one();
    std::cout << "Main thread notified one\n";

    return 0;
}
