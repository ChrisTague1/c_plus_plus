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

void quick_sort(std::vector<int>& arr, int left, int right) {
    int pivot = arr[right];
}

void very_quick_sort(std::vector<int>& arr) {
    if (arr.size() < 2) {
        return;
    }

    quick_sort(arr, 0, arr.size() - 1);
}

/*
5 3 8 6 2 1 0 9 7 4

0 1 2 3 4 5 6 7 8 9
                   
                   
                   
*/

int main() {
    std::vector<int> a = {5, 3, 8, 6, 2, 1, 0, 9, 7, 4};

    print(a);

    my_sort(a);

    print(a);

    return 0;
}