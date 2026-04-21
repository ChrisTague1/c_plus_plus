#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>

std::mutex mtx;
std::condition_variable_any cv;
std::queue<std::string> jobs;

void worker(std::stop_token st) {
    std::unique_lock lock(mtx);

    while (true) {
        cv.wait(lock, st, [] {
            return !jobs.empty();
        });

        if (st.stop_requested() && jobs.empty()) {
            std::cout << "Worker stopping\n";
            break;
        }

        while (!jobs.empty()) {
            std::string job = std::move(jobs.front());
            jobs.pop();

            lock.unlock();

            std::cout << "Processing: " << job << "\n";

            lock.lock();
        }
    }
}

int main() {
    /**
     * Use conditon_variable_any only if you really need stop_tokens
     * Otherwise it is just a little slower.
     * 
     * You can use both with threads/jthreads, but jthreads will auto join and support stop_tokens when needed
     */
    std::jthread t(worker);

    {
        std::lock_guard lock(mtx);
        jobs.push("job 1");
        jobs.push("job 2");
    }
    cv.notify_one();

    {
        std::lock_guard lock(mtx);
        jobs.push("job 3");
    }
    cv.notify_one();

    t.request_stop();
    cv.notify_all();

    return 0;
}