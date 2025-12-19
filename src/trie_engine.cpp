#include "trie_engine.h"

TrieEngine::TrieEngine() {
    root = new TrieNode();
}

void TrieEngine::insert(const string& word) {
    TrieNode* curr = root;
    for (char c : word) {
        if (!curr->children[c])
            curr->children[c] = new TrieNode();
        curr = curr->children[c];
    }
    curr->isEnd = true;
}

void TrieEngine::dfs(TrieNode* node, string prefix, vector<string>& results) {
    if (node->isEnd)
        results.push_back(prefix);

    for (auto& p : node->children) {
        dfs(p.second, prefix + p.first, results);
    }
}

vector<string> TrieEngine::autocomplete(const string& prefix) {
    TrieNode* curr = root;
    for (char c : prefix) {
        if (!curr->children[c])
            return {};
        curr = curr->children[c];
    }

    vector<string> results;
    dfs(curr, prefix, results);
    return results;
}
