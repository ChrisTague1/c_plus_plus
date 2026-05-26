#include <iostream>
#include <queue>
#include <optional>

enum UpOrDown {
    UP,
    DOWN
};

class Elevator {
    std::optional<UpOrDown> status;
    std::priority_queue<int, std::vector<int>, std::less<int>> going_up;
    std::priority_queue<int, std::vector<int>, std::greater<int>> going_down;

   public:
    void add_floor(int n) {

    }
};

class ElevatorSystem {

   public:
    void get_elevator(int n, UpOrDown direction) {

    }
};

int main() {
    std::cout << "hey there" << std::endl;

    return 0;
}