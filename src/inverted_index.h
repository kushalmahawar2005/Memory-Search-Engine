#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct Posting {
    int docID;
    int frequency;
    vector<int> positions;
};

class InvertedIndex {
private:
    unordered_map<string, vector<Posting>> index;
    unordered_map<int, string> docIdToFile;
    int currentDocID;

    string normalize(const string& word);
    vector<string> tokenize(const string& text);

public:
    InvertedIndex();

    void addDocument(const string& filename);
    void printIndex() const;

    // getters for other modules
    const unordered_map<string, vector<Posting>>& getIndex() const;
    const unordered_map<int, string>& getDocMap() const;
};

#endif
