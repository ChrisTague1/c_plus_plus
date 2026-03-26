#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <format>

using namespace std;

struct Strand {
    string dna;
    int start;
    int end;
};

struct GeneDataOriginal {
    string gene;
    int health;
    int index;
};

struct GeneDataProgress {
    string_view gene;
    // string geneOriginal;
    int health;
};

string dna_health(
    const vector<string>& genes,
    const vector<int>& health,
    const vector<Strand>& strands
) {
    vector<int> data;
    unordered_map<char, vector<GeneDataOriginal>> original;
    
    for (int i = 0; i < genes.size(); i++) {
        string gene = genes[i];
        original[gene.front()].push_back(GeneDataOriginal {
            gene,
            health[i],
            i
        });
    }
    
    for (const auto& strand : strands) {
        // cout << strand.dna << " " << strand.start << " " << strand.end << endl;
        unordered_map<char, vector<GeneDataProgress>> progress;
        int total = 0;
        
        for (const char& ch : strand.dna) {
            // cout << "\t" << ch << "\n";
            auto& gene_progress = progress[ch];
            for (const auto& gene : original[ch]) {
                if (gene.index < strand.start || gene.index > strand.end) continue;
                // cout << "\tGene " << gene.gene << " matches!\n";
                gene_progress.push_back(GeneDataProgress {
                    gene.gene,
                    // gene.gene,
                    gene.health
                });
            }
            

            vector<GeneDataProgress> new_progress;
            
            for (auto& gene: gene_progress) {
                gene.gene.remove_prefix(1);
                
                if (gene.gene.empty()) {
                    // cout << "\tEmptied gene " << gene.geneOriginal  << ", adding " << gene.health << " to health\n";
                    total += gene.health;
                } else if (gene.gene.front() == ch) {
                    // cout << "\tFirst char is the same, pushing back\n";
                    new_progress.push_back(gene);
                } else {
                    // cout << "\tMoving to " << gene.gene.front() << "\n";
                    progress[gene.gene.front()].push_back(gene);
                }
            }
            
            progress[ch] = new_progress;
        }
        
        data.push_back(total);
        
        // cout << "\tTotal for " << strand.dna << " is " << total << "\n";
    }
    
    auto [min_it, max_it] = minmax_element(data.begin(), data.end());
    
    
    string s = format("{} {}", *min_it, *max_it);

    return s;
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
