#include <iostream>
#include <vector>
#include <limits>
using namespace std;

// Bellman-Ford algorithm for single-source shortest paths
struct Edge {
    int u, v, w;
};

vector<int> bellman_ford(int n, int src, const vector<Edge>& edges) {
    const int INF = numeric_limits<int>::max() / 2;
    vector<int> dist(n, INF);
    dist[src] = 0;
    for (int i = 1; i < n; ++i) {
        for (const auto& e : edges) {
            if (dist[e.u] < INF && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
            }
        }
    }
    // Detect negative cycle
    for (const auto& e : edges) {
        if (dist[e.u] < INF && dist[e.u] + e.w < dist[e.v]) {
            cout << "Negative weight cycle detected.\n";
            break;
        }
    }
    return dist;
}

int main() {
    int n = 5; // number of vertices
    vector<Edge> edges = {
        {0, 1, -1},
        {0, 2, 4},
        {1, 2, 3},
        {1, 3, 2},
        {1, 4, 2},
        {3, 2, 5},
        {3, 1, 1},
        {4, 3, -3}
    };
    int src = 0;
    vector<int> dist = bellman_ford(n, src, edges);
    cout << "Vertex\tDistance from Source\n";
    for (int i = 0; i < n; ++i) {
        cout << i << "\t";
        if (dist[i] > 1e8) cout << "INF";
        else cout << dist[i];
        cout << endl;
    }
    return 0;
}