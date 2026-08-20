#include <iostream>
#include <vector>

void print(const std::vector<int>& a) {
    for (const auto& n : a) {
        std::cout << n << ", ";
    }
    std::cout << std::endl;
}

void my_sort(std::vector<int>& vec) {
    if (vec.size() < 2) {
        return;
    }

    for (int i = 0; i < vec.size() - 1; ++i) {
        int min = i;

        for (int j = i + 1; j < vec.size(); ++j) {
            if (vec[j] < vec[min]) {
                min = j;
            }
        }

        if (min != i) {
            int temp = vec[i];

            vec[i] = vec[min];
            vec[min] = temp;
        }
    }
}

int main() {
    std::vector<int> a = {5, 3, 8, 6, 2, 1, 0, 9, 7, 4};

    print(a);

    my_sort(a);

    print(a);

    return 0;
}