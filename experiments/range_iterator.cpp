#include <iostream>
#include <iterator>

class RangeIterator {
    int current;

   public:
    RangeIterator(int start) : current(start) {}

    int operator*() const { return current; }

    RangeIterator& operator++() {
        current++;
        return *this;
    }

    bool operator!=(const RangeIterator& other) const {
        return current != other.current;
    }
};

class Range {
    int start, last;

   public:
    Range(int s, int l) : start(s), last(l) {}
    RangeIterator begin() { return RangeIterator(start); }
    RangeIterator end() { return RangeIterator(last); }
};

class AdvancedRangeIterator {
    int current;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = int;
    using pointer = const int*;
    using reference = int;

    AdvancedRangeIterator(int start) : current(start) {}

    int operator*() const { return current; }

    AdvancedRangeIterator& operator++() {
        current++;
        return *this;
    }

    AdvancedRangeIterator& operator--() {
        current--;
        return *this;
    }

    bool operator==(const AdvancedRangeIterator& other) const { return current == other.current; }
    bool operator!=(const AdvancedRangeIterator& other) const { return current != other.current; }
};

int main() {
    // Range r{1, 11};
    // Range r = {1, 11};
    // Range r = Range{1, 11};
    // Range r = Range(1, 11);
    Range r(1, 11);

    for (int i : r) {
        std::cout << i << std::endl;
    }

    return 0;
}