#include "inverted_index.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

using namespace std;

InvertedIndex::InvertedIndex() {
    currentDocID = 0;
}

// Convert word to lowercase and remove punctuation
string InvertedIndex::normalize(const string& word) {
    string result;
    for (char c : word) {
        if (isalpha(c)) {
            result += tolower(c);
        }
    }
    return result;
}

// Split text into normalized tokens
vector<string> InvertedIndex::tokenize(const string& text) {
    vector<string> tokens;
    stringstream ss(text);
    string word;

    while (ss >> word) {
        string clean = normalize(word);
        if (!clean.empty())
            tokens.push_back(clean);
    }
    return tokens;
}

// Read file and build inverted index
void InvertedIndex::addDocument(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    int docID = currentDocID++;
    docIdToFile[docID] = filename;

    string line;
    int position = 0;

    while (getline(file, line)) {
        vector<string> words = tokenize(line);

        for (const string& word : words) {
            bool found = false;

            for (auto& posting : index[word]) {
                if (posting.docID == docID) {
                    posting.frequency++;
                    posting.positions.push_back(position);
                    found = true;
                    break;
                }
            }

            if (!found) {
                Posting p;
                p.docID = docID;
                p.frequency = 1;
                p.positions.push_back(position);
                index[word].push_back(p);
            }

            position++;
        }
    }

    file.close();
}

// -------- Python API methods --------

bool InvertedIndex::containsWord(const string& word) const {
    return index.find(word) != index.end();
}

vector<int> InvertedIndex::getDocuments(const string& word) const {
    vector<int> docs;
    auto it = index.find(word);
    if (it == index.end()) return docs;

    for (const auto& p : it->second)
        docs.push_back(p.docID);

    return docs;
}

vector<int> InvertedIndex::getPositions(const string& word, int docID) const {
    auto it = index.find(word);
    if (it == index.end()) return {};

    for (const auto& p : it->second) {
        if (p.docID == docID)
            return p.positions;
    }
    return {};
}

string InvertedIndex::getFilename(int docID) const {
    auto it = docIdToFile.find(docID);
    if (it == docIdToFile.end()) return "";
    return it->second;
}
