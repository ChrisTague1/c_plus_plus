#include <iostream>
#include <queue>
#include <string>
#include <iterator>
#include <sstream>

using std::cout;
using std::endl;

std::string biggerIsGreater(const std::string& w) {
    std::priority_queue<char> heap; // could use one-hot encoding a 32 bit number to do this instead

    bool found = false;
    int index = -1;

    for (auto rit = w.rbegin(); rit != w.rend(); rit++) {
        heap.push(*rit);

        if (*rit < heap.top()) {
            found = true;
            index = w.size() - 1 - std::distance(w.rbegin(), rit);
            break;
        }
    }

    if (!found) return "no answer";

    std::stringstream ss;

    ss << w.substr(0, index);

    int i;

    for (i = w.size() - 1; w[i] < w[index]; i--); // is this gonna bite my ass?

    ss << w.substr(i, w.size() - 1) << w[index];

    for (i = i - 1; i > index; i--) {
        ss << w[i];
    }

    return ss.str();
}

int main() {
    // std::priority_queue<char> maxHeap;

    // maxHeap.push('a');
    // maxHeap.push('d');
    // maxHeap.push('f');
    // maxHeap.push('c');

    // cout << "Max heap top: " << maxHeap.top() << "\n";

    // maxHeap.pop();

    // cout << "Max heap top: " << maxHeap.top() << "\n";

    cout << biggerIsGreater("zzdkhhc") << "\n";
    cout << biggerIsGreater("ab") << "\n";

    return 0;
}