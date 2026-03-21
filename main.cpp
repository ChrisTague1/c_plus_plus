#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <optional>

template <typename T>
class MyHeap {
   private:
    std::vector<T> data;

    void percolate_up() {
        size_t index = data.size() - 1;
        T value = std::move(data[index]);

        size_t parent_index = (index - 1) / 2;

        while (index != 0) {
            if (value > data[parent_index]) {
                data[index] = std::move(data[parent_index]);
                index = parent_index;
                parent_index = (index - 1) / 2;
            } else {
                break;
            }
        }

        data[index] = value;
    }

    void percolate_down() {
        size_t index = 0;
        size_t temp_index;
        size_t size = data.size();

        T value = std::move(data[index]);

        while (true) {
            std::cout << "In while loop" << "\n";
            temp_index = (2 * index) + 1;

            if (temp_index == size) {
                temp_index = index;
                break;
            };

            size_t right_child = (2 * index) + 2;

            if (right_child < size && data[right_child] > data[temp_index]) {
                temp_index = right_child;
            }

            if (data[temp_index] > value) {
                data[index] = std::move(data[temp_index]);
                index = temp_index;
            } else {
                temp_index = index;
                break;
            }
        }

        data[temp_index] = std::move(value);
    }

   public:
    void push(const T& value) {
        data.push_back(value);
        percolate_up();
    }

    void push(T&& value) {
        data.push_back(std::move(value));
        percolate_up();
    }

    std::optional<T> pop() {
        if (!data.size()) return std::nullopt;

        T value = std::move(data[0]);
        data[0] = std::move(data[data.size() - 1]);
        data.pop_back();

        if (data.size() > 1) {
            percolate_down();
        }

        return value;
    }

    void print() const {
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    MyHeap<int> heap;

    heap.push(1);
    heap.push(2);
    heap.push(3);
    heap.push(2);
    heap.push(4);
    heap.push(5);

    std::optional<int> value;
    value = heap.pop();
    value = heap.pop();

    if (value.has_value()) {
        int printable = value.value();
        std::cout << "Value is " << printable << "\n";
    }

    heap.print();

    return 0;
}