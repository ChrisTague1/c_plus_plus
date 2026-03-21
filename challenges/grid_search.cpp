#include <iostream>
#include <vector>
#include <string>

using namespace std;

// === Original version ===

bool check(const vector<string>& G, const vector<string>& P, int row, int col) {
    if (P.size() + row > G.size()) return false;

    for (int pIndex = 0; pIndex < P.size(); pIndex++) {
        auto find = G[row + pIndex].find(P[pIndex], col);

        if (find != col) return false;
    }

    return true;
}

string gridSearch(const vector<string>& G, const vector<string>& P) {
    for (int i = 0; i < G.size(); i++) {
        int start = 0;

        while (true) {
            auto find = G[i].find(P[0], start);

            if (find == string::npos) break;

            if (check(G, P, i, find)) return "YES";

            start = find + 1;
        }
    }

    return "NO";
}

// === Improved version ===

bool check2(const vector<string>& G, const vector<string>& P, size_t row, size_t col) {
    for (size_t i = 1; i < P.size(); i++) {
        if (G[row + i].compare(col, P[i].size(), P[i]) != 0) return false;
    }
    return true;
}

string gridSearch2(const vector<string>& G, const vector<string>& P) {
    if (P.empty() || G.size() < P.size()) return "NO";

    size_t row_limit = G.size() - P.size();

    for (size_t i = 0; i <= row_limit; i++) {
        for (size_t pos = G[i].find(P[0]); pos != string::npos; pos = G[i].find(P[0], pos + 1)) {
            if (check2(G, P, i, pos)) return "YES";
        }
    }

    return "NO";
}

// === Rabin-Karp hashing version ===

string gridSearch3(const vector<string>& G, const vector<string>& P) {
    if (P.empty() || G.size() < P.size()) return "NO";

    size_t R = G.size(), C = G[0].size();
    size_t r = P.size(), c = P[0].size();
    if (C < c) return "NO";

    const long long BASE = 31;
    const long long MOD = 1e9 + 7;

    // Precompute BASE^(c-1) for rolling hash removal
    long long base_pow = 1;
    for (size_t i = 0; i < c - 1; i++)
        base_pow = (base_pow * BASE) % MOD;

    // row_hashes[i][j] = hash of G[i][j..j+c-1]
    size_t num_windows = C - c + 1;
    vector<vector<long long>> row_hashes(R, vector<long long>(num_windows));

    for (size_t i = 0; i < R; i++) {
        long long h = 0;
        for (size_t k = 0; k < c; k++)
            h = (h * BASE + G[i][k]) % MOD;
        row_hashes[i][0] = h;

        for (size_t j = 1; j < num_windows; j++) {
            h = (h - G[i][j - 1] * base_pow % MOD + MOD) % MOD;
            h = (h * BASE + G[i][j + c - 1]) % MOD;
            row_hashes[i][j] = h;
        }
    }

    // Hash each row of the pattern
    vector<long long> pat_hashes(r);
    for (size_t i = 0; i < r; i++) {
        long long h = 0;
        for (size_t k = 0; k < c; k++)
            h = (h * BASE + P[i][k]) % MOD;
        pat_hashes[i] = h;
    }

    // For each column offset, check consecutive row hashes
    for (size_t j = 0; j < num_windows; j++) {
        for (size_t i = 0; i <= R - r; i++) {
            bool match = true;
            for (size_t k = 0; k < r; k++) {
                if (row_hashes[i + k][j] != pat_hashes[k]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                // Verify to guard against hash collisions
                bool verified = true;
                for (size_t k = 0; k < r; k++) {
                    if (G[i + k].compare(j, c, P[k]) != 0) {
                        verified = false;
                        break;
                    }
                }
                if (verified) return "YES";
            }
        }
    }

    return "NO";
}

int main() {
    vector<string> G = {
        "1234567890",
        "0987654321",
        "1111111111",
        "1111111111",
        "2222222222"
    };
    vector<string> P = {
        "876543",
        "111111",
        "111111"
    };

    cout << "v1: " << gridSearch(G, P) << "\n";
    cout << "v2: " << gridSearch2(G, P) << "\n";
    cout << "v3: " << gridSearch3(G, P) << "\n";
}
