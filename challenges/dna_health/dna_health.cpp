#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <format>

using namespace std;

struct Strand {
    string dna;
    int start;
    int end;
};

struct Node {
    int next[26];
    vector<int> hits;
    int fail;
    int dict;
};

string dna_health(
    const vector<string>& genes,
    const vector<int>& health,
    const vector<Strand>& strands
) {
    vector<Node> trie(1);
    int current;

    for (int i = 0; i < genes.size(); i++) {
        current = 0;
        for (const auto& ch : genes[i]) {
            if (trie[current].next[ch - 'a']) {
                current = trie[current].next[ch - 'a'];
            } else {
                trie.push_back({});
                trie[current].next[ch - 'a'] = trie.size() - 1;
                current = trie.size() - 1;
            }
        }
        trie[current].hits.push_back(i);
    }

    queue<int> nodeQueue;
    nodeQueue.push(0);

    while (!nodeQueue.empty()) {
        int nodeIndex = nodeQueue.front();
        nodeQueue.pop();

        for (int i = 0; i < 26; i++) {
            if (trie[nodeIndex].next[i]) {
                Node& node = trie[trie[nodeIndex].next[i]];

                node.fail = nodeIndex == 0 ? 0 : trie[trie[nodeIndex].fail].next[i];
                node.dict = trie[node.fail].hits.size() ? node.fail : trie[node.fail].dict;

                nodeQueue.push(trie[nodeIndex].next[i]);
            } else {
                trie[nodeIndex].next[i] = nodeIndex == 0 ? 0 : trie[trie[nodeIndex].fail].next[i];
            }
        }
    }

    vector<int> totals;

    for (const auto& strand : strands) {
        int total = 0;
        current = 0;

        for (const auto& ch : strand.dna) {
            current = trie[current].next[ch - 'a'];

            for (const auto& hit : trie[current].hits) {
                if (hit >= strand.start && hit <= strand.end) {
                    total += health[hit];
                }
            }

            if (trie[current].dict) {
                int dictIndex = trie[current].dict;
                while (dictIndex) {
                    for (const auto& hit : trie[dictIndex].hits) {
                        if (hit >= strand.start && hit <= strand.end) {
                            total += health[hit];
                        }
                    }
                    dictIndex = trie[dictIndex].dict;
                }
            }
        }

        totals.push_back(total);
    }

    auto [min_it, max_it] = minmax_element(totals.begin(), totals.end());
    return format("{} {}", *min_it, *max_it);
}

struct TestCase {
    vector<string> genes;
    vector<int> health;
    vector<Strand> strands;
    string answer;
};

int main() {
    vector<TestCase> tests = {
        {
            {"a", "b", "c", "aa", "d", "b"},
            {1, 2, 3, 4, 5, 6},
            {
                {"caaab", 1, 5},
                {"xyz", 0, 4},
                {"bcdybc", 2, 4},
            },
            "0 19"
        },
    };

    for (size_t i = 0; i < tests.size(); i++) {
        string result = dna_health(tests[i].genes, tests[i].health, tests[i].strands);
        if (result == tests[i].answer) {
            cout << "Test " << i << ": PASS\n";
        } else {
            cout << "Test " << i << ": FAIL (expected \"" << tests[i].answer << "\", got \"" << result << "\")\n";
        }
    }
}
