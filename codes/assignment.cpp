#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

// Hungarian Algorithm for Assignment Problem (Min Cost)
const int INF = numeric_limits<int>::max();

int assignment(const vector<vector<int>>& cost, vector<int>& row_match, vector<int>& col_match) {
    int n = cost.size();
    vector<int> u(n+1), v(n+1), p(n+1), way(n+1);
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        vector<int> minv(n+1, INF);
        vector<bool> used(n+1, false);
        int j0 = 0;
        do {
            used[j0] = true;
            int i0 = p[j0], delta = INF, j1 = 0;
            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    int cur = cost[i0-1][j-1] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= n; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    row_match.assign(n, -1);
    col_match.assign(n, -1);
    for (int j = 1; j <= n; ++j) {
        if (p[j] > 0 && p[j] <= n) {
            row_match[p[j]-1] = j-1;
            col_match[j-1] = p[j]-1;
        }
    }
    return -v[0];
}

int main() {
    // Example cost matrix (minimize total cost)
    vector<vector<int>> cost = {
        {9, 2, 7, 8},
        {6, 4, 3, 7},
        {5, 8, 1, 8},
        {7, 6, 9, 4}
    };
    int n = cost.size();
    vector<int> row_match, col_match;
    int min_cost = assignment(cost, row_match, col_match);

    cout << "Minimum assignment cost: " << min_cost << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Row " << i << " assigned to Column " << row_match[i] << endl;
    }
    return 0;
}