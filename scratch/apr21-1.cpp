#include <iostream>
#include <deque>

int main() {
    std::deque<int> dq;

    dq.push_back(10);
    dq.push_front(5);
    dq.emplace_back(20);

    std::cout << dq.front() << "\n";
    std::cout << dq.front() << "\n";
    std::cout << dq.back() << "\n";

    dq.pop_front();
    dq.pop_back();

    for (int x : dq) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}