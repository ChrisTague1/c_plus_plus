#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
   public:
    vector<int> gcdValues(const vector<int>& nums, const vector<long long>& queries) {
        int n = static_cast<int>(nums.size());

        int largest = -1;

        unordered_map<int, int> counts;
        set<int, greater<int>> uniques;

        for (int i = 0; i < n; ++i) {
            largest = max(largest, nums[i]);
            uniques.insert(nums[i]);
            ++counts[nums[i]];
        }

        unordered_map<int, int> pairings;

        for (int i = largest; i > 0; --i) {
            auto it = counts.find(i);

            if (it == counts.end()) continue;

            pairings[it->first] = it->second * (it->second - 1) / 2;

            for (int multiple = 2; multiple * i <= largest; multiple++) {
                auto it = counts.find(multiple * i);

                if (it == counts.end()) continue;

                pairings[it->first] -= it->second * (it->second - 1) / 2;
            }
        }

        return {};
    }
};
