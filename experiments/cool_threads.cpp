#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::jthread t([](std::stop_token st) {
        while (!st.stop_requested()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Working...\n";
        }
        std::cout << "Stopping...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "Stopped.\n";
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Asking it to stop...\n";

    t.request_stop();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Everything is done now\n";

    return 0;
}