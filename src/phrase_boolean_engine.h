#ifndef PHRASE_BOOLEAN_ENGINE_H
#define PHRASE_BOOLEAN_ENGINE_H

#include "inverted_index.h"
#include <vector>
#include <string>

using namespace std;

class PhraseBooleanEngine {
private:
    InvertedIndex& index;

public:
    PhraseBooleanEngine(InvertedIndex& idx);

    vector<int> phraseSearch(const vector<string>& phrase);
    vector<int> booleanAND(const vector<string>& terms);
    vector<int> booleanOR(const vector<string>& terms);
    vector<int> booleanNOT(const string& term, int totalDocs);
};

#endif
