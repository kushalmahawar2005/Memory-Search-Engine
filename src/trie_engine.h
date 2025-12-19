#ifndef TRIE_ENGINE_H
#define TRIE_ENGINE_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct TrieNode {
    bool isEnd;
    unordered_map<char, TrieNode*> children;

    TrieNode() : isEnd(false) {}
};

class TrieEngine {
private:
    TrieNode* root;

    void dfs(TrieNode* node, string prefix, vector<string>& results);

public:
    TrieEngine();
    void insert(const string& word);
    vector<string> autocomplete(const string& prefix);
};

#endif
