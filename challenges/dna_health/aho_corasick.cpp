#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <optional>

using namespace std;

struct Node {
    int next[26];
    optional<string> word;
    int fail;
    int dict;
    bool isEnd;
};

vector<string> the_algo(const vector<string>& dict, const string& input) {
    vector<Node> trie(1);
    int current;


    for (const auto& item : dict) {
        current = 0;
        for (const auto& ch : item) {
            if (trie[current].next[ch - 'a']) {
                current = trie[current].next[ch - 'a'];
            } else {
                trie.push_back({});
                trie[current].next[ch - 'a'] = trie.size() - 1;
                current = trie.size() - 1;
            }
        }

        trie[current].isEnd = true;
        trie[current].word = item;
    }

    queue<int> nodeQueue;
    nodeQueue.push(0);

    while (!nodeQueue.empty()) {
        int nodeIndex = nodeQueue.front();
        nodeQueue.pop();

        for (char i = 0; i < 26; i++) {
            if (trie[nodeIndex].next[i]) {
                Node& n = trie[trie[nodeIndex].next[i]];

                n.fail = nodeIndex == 0 ? 0 : trie[trie[nodeIndex].fail].next[i];
                
                n.dict = trie[n.fail].isEnd ? n.fail : trie[n.fail].dict;

                nodeQueue.push(trie[nodeIndex].next[i]);
            } else {
                trie[nodeIndex].next[i] = nodeIndex == 0 ? 0 : trie[trie[nodeIndex].fail].next[i];
            }
        }
    }

    for (int i = 0; i < trie.size(); i++) {
        cout << i << ":\n";

        cout << "\t";
        for (const auto& item : trie[i].next) {
            cout << item << " ";
        }
        cout << "\n";

        cout << "\t" << trie[i].fail << "\n";
        cout << "\t" << trie[i].dict << "\n";

        if (trie[i].isEnd) {
            cout << "\t" << trie[i].word.value() << "\n";
        }
    }

    vector<string> output;

    current = 0;
    for (const auto& ch : input) {
        current = trie[current].next[ch - 'a'];

        if (trie[current].isEnd) {
            output.push_back(trie[current].word.value());
        }

        if (trie[current].dict) {
            int dict = trie[current].dict;

            while (dict) {
                output.push_back(trie[dict].word.value());
                dict = trie[dict].dict;
            }
        }
    }

    return output;
}

struct TestCase {
    vector<string> dict;
    vector<string> output;
    string input;
};

int main() {
    vector<int> a = {1, 2, 3};

    vector<TestCase> testCases = {
        {
            {"a", "ab", "bab", "bc", "bca", "c", "caa"},
            {"a", "ab", "bc", "c", "c", "a", "ab"},
            "abccab"
        }
    };

    for (const auto& testCase : testCases) {
        auto result = the_algo(testCase.dict, testCase.input);

        if (result != testCase.output) {
            cout << "Failed!" << endl;
        } else {
            cout << "Passed!" << endl;
        }
    }

    return 0;
}