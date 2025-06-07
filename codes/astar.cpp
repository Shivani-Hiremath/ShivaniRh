#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <utility>
#include <algorithm>
using namespace std;

// A* Algorithm for grid-based shortest path

struct Node {
    int x, y;
    int g, h, f;
    Node* parent;
    Node(int x, int y, int g, int h, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
};

struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->f > b->f;
    }
};

int heuristic(int x1, int y1, int x2, int y2) {
    // Manhattan distance
    return abs(x1 - x2) + abs(y1 - y2);
}

vector<pair<int, int>> reconstruct_path(Node* end) {
    vector<pair<int, int>> path;
    for (Node* node = end; node != nullptr; node = node->parent)
        path.push_back({node->x, node->y});
    reverse(path.begin(), path.end());
    return path;
}

vector<pair<int, int>> astar(const vector<vector<int>>& grid, pair<int, int> start, pair<int, int> goal) {
    int n = grid.size(), m = grid[0].size();
    vector<vector<bool>> closed(n, vector<bool>(m, false));
    priority_queue<Node*, vector<Node*>, CompareNode> open;
    open.push(new Node(start.first, start.second, 0, heuristic(start.first, start.second, goal.first, goal.second)));

    while (!open.empty()) {
        Node* curr = open.top(); open.pop();
        if (curr->x == goal.first && curr->y == goal.second) {
            auto path = reconstruct_path(curr);
            // Clean up memory
            while (!open.empty()) { delete open.top(); open.pop(); }
            return path;
        }
        closed[curr->x][curr->y] = true;
        static const int dx[] = {-1, 1, 0, 0};
        static const int dy[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; ++d) {
            int nx = curr->x + dx[d], ny = curr->y + dy[d];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && !closed[nx][ny] && grid[nx][ny] == 0) {
                int g = curr->g + 1;
                int h = heuristic(nx, ny, goal.first, goal.second);
                open.push(new Node(nx, ny, g, h, curr));
            }
        }
    }
    return {}; // No path found
}

int main() {
    vector<vector<int>> grid = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0},
        {0, 0, 0, 1, 0}
    };
    pair<int, int> start = {0, 0}, goal = {4, 4};
    auto path = astar(grid, start, goal);
    if (!path.empty()) {
        cout << "Path found:\n";
        for (auto [x, y] : path)
            cout << "(" << x << "," << y << ") ";
        cout << endl;
    } else {
        cout << "No path found.\n";
    }
    return 0;
}