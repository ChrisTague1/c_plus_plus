#include <vector>

/*
& - lvalue reference, has a name and address, persists
&& - rvalue reference, somethiing temporary, okay to steal from

int x = 42;
    ^ lvalue
         ^ rvalue

*/

class Grid {
    std::vector<std::vector<int>> cells;
    int rows;
    int cols;

   public:
    Grid(Grid&& other) noexcept
        : cells(std::move(other.cells)), rows(other.rows), cols(other.cols) {
        /*
        my_func(std::move(grid)) will call this
        */
        other.rows = 0;
        other.cols = 0;
    }

    Grid& operator=(Grid&& other) noexcept {
        /*
        FYI don't need to write these, the compiler will create good move constructors
        for lang default types
        */
        cells = std::move(other.cells);
        rows = other.rows;
        cols = other.cols;
        other.rows = 0;
        other.cols = 0;
        return *this;
    }
};


int main() {
    return 0;
}