#include <iostream>
#include <vector>
#include <utility>
#include <string>

template <typename T>
class MyHeap {
   private:
    std::vector<T> data;

    void percolate_up() {

        T value = std::move(data[index]);

        size_t parent_index = (index - 1) / 2;

        while (index != 0) {
            if (value > data[parent_index]) {
                data[index] = std::move(data[parent_index]);
                index = parent_index;
                parent_index = (index - 1) / 2
            } else {
                break;
            }
        }

        data[index] = value;
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

    heap.print();

    return 0;
}