#include <iostream>
#include <vector>

template <typename T>
class MyHeap {
   private:
    std::vector<T> data;

    // "Hole" technique: instead of swapping at every level,
    // save the value, slide parents down, place once at the end.
    void percolate_up(size_t index) {
        T value = std::move(data[index]);

        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (value < data[parent]) {
                data[index] = std::move(data[parent]);
                index = parent;
            } else {
                break;
            }
        }

        data[index] = std::move(value);
    }

    // Same idea downward: move the hole down without swapping,
    // then place the value once at its final position.
    void percolate_down(size_t index) {
        T value = std::move(data[index]);
        size_t size = data.size();

        while (true) {
            size_t left = 2 * index + 1;
            if (left >= size) break;

            size_t right = left + 1;
            size_t smallest = (right < size && data[right] < data[left]) ? right : left;

            if (data[smallest] < value) {
                data[index] = std::move(data[smallest]);
                index = smallest;
            } else {
                break;
            }
        }

        data[index] = std::move(value);
    }

   public:
    MyHeap() = default;

    // Bulk construction via heapify — O(n) instead of O(n log n)
    MyHeap(std::vector<T> input) : data(std::move(input)) {
        if (data.size() <= 1) return;
        for (size_t i = data.size() / 2; i-- > 0;) {
            percolate_down(i);
        }
    }

    void reserve(size_t capacity) { data.reserve(capacity); }

    void put(T value) {
        data.push_back(std::move(value));
        percolate_up(data.size() - 1);
    }

    T pop() {
        T result = std::move(data[0]);
        data[0] = std::move(data.back());
        data.pop_back();

        if (!data.empty()) {
            percolate_down(0);
        }

        return result;
    }

    std::optional<T> peak() const {
        if (!data.empty()) {
            return data[0];
        }
        return std::nullopt;
    }

    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }

    MyHeap& operator<<(T value) {
        put(std::move(value));
        return *this;
    }

    MyHeap& operator>>(T& value) {
        value = pop();
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyHeap& heap) {
        os << "( ";

        bool first = true;
        for (const auto& item : heap.data) {
            if (!first) {
                os << ", ";
            }
            first = false;
            os << item;
        }

        os << " )";
        return os;
    }
};

int main() {
    // Bulk heapify construction
    MyHeap<int> heap({5, 3, 8, 1, 4, 2, 7});

    std::cout << heap << "\n";

    int value;
    heap >> value;

    std::cout << "popped: " << value << "\n";
    std::cout << heap << "\n";

    return 0;
}
