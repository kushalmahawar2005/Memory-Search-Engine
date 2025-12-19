#include "inverted_index.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

using namespace std;

InvertedIndex::InvertedIndex() {
    currentDocID = 0;
}

string InvertedIndex::normalize(const string& word) {
    string result;
    for (char c : word) {
        if (isalpha(c)) {
            result += tolower(c);
        }
    }
    return result;
}

vector<string> InvertedIndex::tokenize(const string& text) {
    vector<string> tokens;
    stringstream ss(text);
    string word;

    while (ss >> word) {
        string cleanWord = normalize(word);
        if (!cleanWord.empty()) {
            tokens.push_back(cleanWord);
        }
    }
    return tokens;
}

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

void InvertedIndex::printIndex() const {
    for (const auto& pair : index) {
        cout << pair.first << " -> ";
        for (const auto& posting : pair.second) {
            cout << "[Doc " << posting.docID
                 << ", Freq " << posting.frequency << "] ";
        }
        cout << endl;
    }
}

const unordered_map<string, vector<Posting>>& InvertedIndex::getIndex() const {
    return index;
}

const unordered_map<int, string>& InvertedIndex::getDocMap() const {
    return docIdToFile;
}
