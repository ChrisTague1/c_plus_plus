#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

constexpr int MAC_CACHE_SIZE = 128; // is 128 in reality
/**
 * ^^^ if the cache sized is messed up you can make the program 20x slower
 * Putting 32 there instead of the 128 that it is in reality makes CooledData the slowest
 */

const long int ITERATIONS = 100'000'000;

// These will be in the same cache line, on mac you can check with "sysctl hw.cachelinesize"
// It is 128 bytes on a base M4, so these are definitely on the same cache line
struct HotData {
    std::atomic<int> a; // 4 bytes
    std::atomic<int> b; // 4 bytes
};

struct CooledData {
    alignas(MAC_CACHE_SIZE) std::atomic<int> a;
    alignas(MAC_CACHE_SIZE) std::atomic<int> b;
};

void work_atomic(std::atomic<int>& counter) {
    for (long int i = 0; i < ITERATIONS; ++i) counter++;
}

void work_single(int& a, int& b) {
    for (long int i = 0; i < ITERATIONS; ++i) a++;
    for (long int i = 0; i < ITERATIONS; ++i) b++;
}

int main() {
    /**
     * When you do value++, what happens is:
     * - Read a value from memory into a register
     * - Increment the register by 1
     * - Write it back to memory
     * atomic<> says that all operations on this should be done in a unit; one thread at a time
     */
    std::cout << "Size of HotData: " << sizeof(HotData) << " bytes\n";
    std::cout << "Size of CooledData: " << sizeof(CooledData) << " bytes" << std::endl;

    HotData hot;
    CooledData cool;
    int sa = 0, sb = 0;

    // --- TEST 1: SINGLE THREADED (No Atomics, No Threads) ---
    auto start = std::chrono::high_resolution_clock::now();
    work_single(sa, sb);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Single Threaded (Normal int): " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;

    // --- TEST 2: HOT DATA (Two Threads, False Sharing) ---
    start = std::chrono::high_resolution_clock::now();
    std::thread t1(work_atomic, std::ref(hot.a));
    std::thread t2(work_atomic, std::ref(hot.b));
    t1.join(); t2.join();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "HotData (False Sharing):      " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;

    // --- TEST 3: COOLED DATA (Two Threads, Aligned) ---
    start = std::chrono::high_resolution_clock::now();
    std::thread t3(work_atomic, std::ref(cool.a));
    std::thread t4(work_atomic, std::ref(cool.b));
    t3.join(); t4.join();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "CooledData (Aligned):         " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;

    return 0;
}