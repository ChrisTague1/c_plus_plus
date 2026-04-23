#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Atomics vs mutexes:
//
// std::atomic<T> is for one object whose reads/writes/updates must be indivisible.
// std::mutex is for protecting a critical section, often multiple objects or a
// sequence of operations that must stay consistent as a group.
//
// Valid std::atomic<T> types:
// - bool
// - integral types: int, long, std::size_t, char, etc.
// - pointer types: T*
// - floating point types in modern C++ standards
// - enum types, if they satisfy the primary-template rules
// - your own trivially copyable types, if T is not cv-qualified and is copy/move
//   constructible/assignable. These may not be lock-free.
// - std::shared_ptr<T> and std::weak_ptr<T> have atomic specializations.
//
// Common std::atomic methods:
// - load()
// - store(value)
// - exchange(value)
// - compare_exchange_weak(expected, desired)
// - compare_exchange_strong(expected, desired)
// - is_lock_free()
// - wait(old_value), notify_one(), notify_all() in C++20
//
// Extra methods/operators on integral atomics:
// - fetch_add(), fetch_sub(), fetch_and(), fetch_or(), fetch_xor()
// - ++, --, +=, -=, &=, |=, ^=
//
// Extra methods/operators on pointer atomics:
// - fetch_add(), fetch_sub()
// - ++, --, +=, -=
//
// Each operation can also take a std::memory_order. The default is
// std::memory_order_seq_cst, which is the simplest and strongest ordering.

constexpr int THREADS = 8;
constexpr int INCREMENTS_PER_THREAD = 100'000;

void atomic_counter_demo() {
    std::atomic<int> counter = 0;
    std::vector<std::jthread> workers;

    for (int i = 0; i < THREADS; ++i) {
        workers.emplace_back([&] {
            for (int j = 0; j < INCREMENTS_PER_THREAD; ++j) {
                ++counter;
            }
        });
    }

    // jthread joins automatically when workers is destroyed at the end of scope.
    workers.clear();

    std::cout << "Atomic counter:\n";
    std::cout << "  expected: " << THREADS * INCREMENTS_PER_THREAD << '\n';
    std::cout << "  actual:   " << counter.load() << "\n\n";
}

void mutex_counter_demo() {
    int counter = 0;
    std::mutex counter_mtx;
    std::vector<std::jthread> workers;

    for (int i = 0; i < THREADS; ++i) {
        workers.emplace_back([&] {
            for (int j = 0; j < INCREMENTS_PER_THREAD; ++j) {
                std::lock_guard lock(counter_mtx);
                ++counter;
            }
        });
    }

    workers.clear();

    std::cout << "Mutex counter:\n";
    std::cout << "  expected: " << THREADS * INCREMENTS_PER_THREAD << '\n';
    std::cout << "  actual:   " << counter << "\n\n";
}

void mutex_invariant_demo() {
    std::mutex mtx;
    std::vector<std::string> messages;
    int message_count = 0;
    std::vector<std::jthread> workers;

    for (int id = 0; id < THREADS; ++id) {
        workers.emplace_back([&, id] {
            for (int i = 0; i < 5; ++i) {
                std::lock_guard lock(mtx);

                // These two updates describe one logical change. A mutex keeps
                // the vector and count consistent with each other.
                messages.push_back("thread " + std::to_string(id) +
                                   " message " + std::to_string(i));
                ++message_count;
            }
        });
    }

    workers.clear();

    std::cout << "Mutex protecting a multi-object invariant:\n";
    std::cout << "  messages.size(): " << messages.size() << '\n';
    std::cout << "  message_count:   " << message_count << "\n\n";
}

void compare_exchange_demo() {
    std::atomic<int> state = 0;
    int expected = 0;

    bool changed = state.compare_exchange_strong(expected, 1);

    std::cout << "compare_exchange_strong:\n";
    std::cout << "  changed from 0 to 1: " << std::boolalpha << changed << '\n';
    std::cout << "  state:               " << state.load() << "\n\n";
}

int main() {
    atomic_counter_demo();
    mutex_counter_demo();
    mutex_invariant_demo();
    compare_exchange_demo();

    std::cout << "Rule of thumb:\n";
    std::cout << "  Use atomic for one independent value, like a flag or counter.\n";
    std::cout << "  Use a mutex when multiple values or multiple steps must agree.\n";
}
