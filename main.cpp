#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<typename T>
string stringify_vector(const vector<T>& v) {
    string result;

    result += "( ";
    for (size_t i = 0; i < v.size() ; i++) {
        result += to_string(v[i]);
        result += " ";
    }
    result += ")";

    return result;
}

int main() {
    vector<int> v = {1, 2, 3};

    cout << stringify_vector(v) << "\n";

    return 0;
}