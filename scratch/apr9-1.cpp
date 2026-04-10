#include <iostream>
#include <vector>
#include <thread>
#include <latch>
#include <barrier>
#include <atomic>
#include <chrono>

using namespace std::chrono_literals;

template <typename BarrierType>
void parallel_work(
    int id,
    std::latch& start_latch,
    BarrierType& phase_barrier,
    std::atomic<int>& completion_count
) {
    std::cout << "Thread " << id << " initialized\n";
    start_latch.count_down();

    std::this_thread::sleep_for(100ms * id);
    std::cout << "Thread " << id << " reached the barrier\n";

    phase_barrier.arrive_and_wait();

    std::cout << "Thread " << id << " starting phase 2\n";

    completion_count.fetch_add(1);
    completion_count.notify_one();
}

int main() {
    const int num_threads = 3;
    std::atomic<int> completion_count{0};

    std::latch start_sync{num_threads};
    std::barrier phase_sync{num_threads, []() noexcept {
        std::cout << "ALl threads reached barrier: Moving to phase 2";
    }};

    std::vector<std::jthread> workers;

    for (int i = 0; i < num_threads; ++i) {
        workers.emplace_back([i, &start_sync, &phase_sync, &completion_count]() {
            parallel_work(i, start_sync, phase_sync, completion_count);
        });
    }

    start_sync.wait();
    std::cout << "Main: All threads report ready\n";

    while (completion_count.load() < num_threads) {
        completion_count.wait(completion_count.load());
    }

    std::cout << "Main: All work is done, threads will auto destroy\n";

    return 0;
}