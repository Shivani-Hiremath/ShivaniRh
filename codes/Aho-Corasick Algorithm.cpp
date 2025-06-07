#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Aho-Corasick algorithm for multiple pattern matching
class AhoCorasick {
private:
    struct Node {
        int next[26];         // Next state for each character
        int fail;             // Failure link
        vector<int> output;   // Output pattern indices at this node
        Node() : fail(0) {
            fill(begin(next), end(next), -1);
        }
    };

    vector<Node> trie;
    int root;
    vector<string> patterns;

public:
    AhoCorasick() : root(0) {
        trie.push_back(Node());
    }

    // Insert a pattern into the trie
    void insert(const string& pattern) {
        int current = root;
        for (char c : pattern) {
            int index = c - 'a';
            if (trie[current].next[index] == -1) {
                trie[current].next[index] = trie.size();
                trie.push_back(Node());
            }
            current = trie[current].next[index];
        }
        trie[current].output.push_back(patterns.size());
        patterns.push_back(pattern);
    }

    // Build the failure links
    void build() {
        queue<int> q;
        for (int i = 0; i < 26; ++i) {
            if (trie[root].next[i] != -1) {
                trie[trie[root].next[i]].fail = root;
                q.push(trie[root].next[i]);
            } else {
                trie[root].next[i] = root;
            }
        }

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int i = 0; i < 26; ++i) {
                int child = trie[current].next[i];
                if (child != -1) {
                    int fail = trie[current].fail;
                    while (trie[fail].next[i] == root && fail != root)
                        fail = trie[fail].fail;
                    trie[child].fail = trie[fail].next[i];
                    trie[child].output.insert(
                        trie[child].output.end(),
                        trie[trie[child].fail].output.begin(),
                        trie[trie[child].fail].output.end()
                    );
                    q.push(child);
                } else {
                    trie[current].next[i] = trie[trie[current].fail].next[i];
                }
            }
        }
    }

    // Search for patterns in the text
    vector<pair<int, string>> search(const string& text) {
        vector<pair<int, string>> results;
        int state = root;
        for (int i = 0; i < text.size(); ++i) {
            int index = text[i] - 'a';
            state = trie[state].next[index];
            for (int patIdx : trie[state].output) {
                results.emplace_back(i - patterns[patIdx].size() + 1, patterns[patIdx]);
            }
        }
        return results;
    }
};

// Example usage
int main() {
    AhoCorasick ac;
    ac.insert("he");
    ac.insert("she");
    ac.insert("his");
    ac.insert("hers");
    ac.build();

    string text = "ahishers";
    auto matches = ac.search(text);
    for (auto& match : matches) {
        cout << "Pattern \"" << match.second << "\" found at index " << match.first << endl;
    }
    return 0;
}


