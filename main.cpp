#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

template<typename T>
string stringify_vector(const vector<T>& v) {
    stringstream ss;
    ss << "( ";
    for (const auto& item : v) {
        ss << item << " ";
    }
    ss << ")";
    return ss.str();
}

int main() {
    vector<int> v = {1, 2, 3};

    cout << stringify_vector(v) << "\n";

    return 0;
}