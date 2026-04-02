#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

vector<int> absolutePerms(int n, int k) {
    if (k == 0) {
        vector<int> output;
        output.reserve(n);
        
        for (int i = 1; i <= n; ++i) {
            output.push_back(i);
        }
        
        return output;
    }

    if (n == 1) {
        return {-1};
    }
    
    if (2 * k % n != 0) return {-1};
    
    vector<int> output;
    output.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        // cout << "i = " << i << endl;

        int bigGroup = i / (2 * k);
        // cout << "bigGroup = " << bigGroup << endl;

        int smallGroup = i % (2 * k) / k;
        // cout << "smallGroup = " << smallGroup << endl;

        int smallerGroup = i % k;
        // cout << "smallerGroup = " << smallerGroup << endl;

        int out = 2 * k * bigGroup + smallerGroup + 1;
        out += smallGroup == 0 ? k : 0;
        // cout << "out = " << out << endl;
        
        output.push_back(out);

        // cout << endl;
    }
    
    return output;
}

struct TestCase {
    int n;
    int k;
    vector<int> output;
};

int main() {
    vector<TestCase> testCases = {
        {
            2,
            1,
            {2, 1}
        },
        {
            10,
            5,
            {6, 7, 8, 9, 10, 1, 2, 3, 4, 5}
        },
        {
            1,
            0,
            {1}
        },
        {
            1,
            1,
            {-1}
        }
    };

    int passed = 0;
    int failed = 0;

    for (int t = 0; t < (int)testCases.size(); ++t) {
        const auto& test = testCases[t];
        auto output = absolutePerms(test.n, test.k);

        cout << "--- Test " << t + 1 << " (n=" << test.n << ", k=" << test.k << ") ---" << endl;

        if (output != test.output) {
            ++failed;
            cout << "  FAILED" << endl;
            cout << "  Expected: [";
            for (int i = 0; i < (int)test.output.size(); ++i)
                cout << (i ? ", " : "") << test.output[i];
            cout << "]" << endl;
            cout << "  Got:      [";
            for (int i = 0; i < (int)output.size(); ++i)
                cout << (i ? ", " : "") << output[i];
            cout << "]" << endl;
        } else {
            ++passed;
            cout << "  PASSED" << endl;
        }
    }

    cout << "\n=== Results: " << passed << " passed, " << failed << " failed, "
         << testCases.size() << " total ===" << endl;

    return 0;
}