#include <iostream>
#include <vector>

template <typename T>
class MyHeap {
   private:
    std::vector<T> data;

    void percolate_up(size_t index) {
        if (index == 0) return;

        size_t parentIndex = (index - 1) / 2;

        if (data[index] < data[parentIndex]) {
            T temp = data[index];
            data[index] = data[parentIndex];
            data[parentIndex] = temp;

            percolate_up(parentIndex);
        }

        return;
    }

    void percolate_down(size_t index) {
        size_t left_child_index = 2 * index + 1;
        size_t right_child_index = 2 * index + 2;

        if (left_child_index >= data.size()) return;

        size_t temp_index;

        if (right_child_index >= data.size()) {
            temp_index = left_child_index;
        } else {
            if (data[left_child_index] < data[right_child_index]) {
                temp_index = left_child_index;
            } else {
                temp_index = right_child_index;
            }
        }

        if (data[temp_index] < data[index]) {
            T temp = data[temp_index];
            data[temp_index] = data[index];
            data[index] = temp;

            percolate_down(temp_index);
        }

        return;
    }

   public:
    void put(T value) {
        data.push_back(value);
        size_t index = data.size();
        percolate_up(index - 1);
    }

    T pop() {
        size_t size = data.size();
        T temp = data[0];
        data[0] = data[size - 1];
        data[size - 1] = temp;
        data.pop_back();

        if (data.size()) {
            percolate_down(0);
        }

        return temp;
    }

    std::optional<T> peak() {
        if (data.size()) {
            return data[0];
        }

        return std::nullopt;
    }

    MyHeap& operator<<(const T& value) {
        put(value);
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
    MyHeap<int> heap;

    heap << 1 << 4 << 5 << 2;

    std::cout << heap << "\n";

    int value;

    heap >> value;

    std::cout << value << "\n";

    return 0;
}