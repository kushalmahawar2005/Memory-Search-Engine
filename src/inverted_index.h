#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Stores information of a word in a document
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

    // Core indexing
    void addDocument(const string& filename);

    // ---- Python-safe API methods (READ-ONLY) ----
    bool containsWord(const string& word) const;
    vector<int> getDocuments(const string& word) const;
    vector<int> getPositions(const string& word, int docID) const;

    // Metadata
    string getFilename(int docID) const;
};

#endif
