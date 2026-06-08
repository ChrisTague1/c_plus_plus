#include <cassert>
#include <iostream>
#include <vector>

int binarySearch(const std::vector<int>& nums, int target) {
    if (nums.size() == 0) return -1;

    int left = -1;
    int right = nums.size();
    int mid;

    while (true) {
        mid = (left + right) / 2;

        if (nums[mid] == target) {
            return mid;
        }

        if (left == mid || right == mid) {
            return -1;
        }

        if (nums[mid] < target) {
            left = mid;
        } else {
            right = mid;
        }
    }
}

int main() {
    std::cout << (-1 / 2 == 0) << std::endl;
    

    std::vector<int> nums = {1, 3, 5, 7, 9, 11};

    assert(binarySearch(nums, 1) == 0);
    assert(binarySearch(nums, 7) == 3);
    assert(binarySearch(nums, 11) == 5);

    assert(binarySearch(nums, 0) == -1);
    assert(binarySearch(nums, 6) == -1);
    assert(binarySearch(nums, 12) == -1);

    std::vector<int> oneItem = {42};
    assert(binarySearch(oneItem, 42) == 0);
    assert(binarySearch(oneItem, 7) == -1);

    std::vector<int> duplicates = {2, 4, 4, 4, 8};
    int duplicateIndex = binarySearch(duplicates, 4);
    assert(duplicateIndex >= 1 && duplicateIndex <= 3);

    std::vector<int> empty;
    assert(binarySearch(empty, 10) == -1);

    std::cout << "All binary search tests passed.\n";

    return 0;
}
