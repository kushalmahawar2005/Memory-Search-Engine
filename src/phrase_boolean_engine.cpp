#include "phrase_boolean_engine.h"
#include <set>

using namespace std;

PhraseBooleanEngine::PhraseBooleanEngine(InvertedIndex& idx)
    : index(idx) {}

// -------- Phrase Search --------
vector<int> PhraseBooleanEngine::phraseSearch(const vector<string>& phrase) {
    if (phrase.empty()) return {};

    vector<int> candidateDocs = index.getDocuments(phrase[0]);
    vector<int> result;

    for (int docID : candidateDocs) {
        vector<int> prevPositions = index.getPositions(phrase[0], docID);
        bool match = true;

        for (int i = 1; i < phrase.size(); i++) {
            vector<int> currPositions = index.getPositions(phrase[i], docID);
            vector<int> nextPositions;

            for (int pos : prevPositions) {
                for (int currPos : currPositions) {
                    if (currPos == pos + 1)
                        nextPositions.push_back(currPos);
                }
            }

            if (nextPositions.empty()) {
                match = false;
                break;
            }
            prevPositions = nextPositions;
        }

        if (match)
            result.push_back(docID);
    }

    return result;
}

// -------- Boolean AND --------
vector<int> PhraseBooleanEngine::booleanAND(const vector<string>& terms) {
    if (terms.empty()) return {};

    set<int> result(index.getDocuments(terms[0]).begin(),
                    index.getDocuments(terms[0]).end());

    for (int i = 1; i < terms.size(); i++) {
        set<int> curr(index.getDocuments(terms[i]).begin(),
                      index.getDocuments(terms[i]).end());

        set<int> intersection;
        for (int d : result) {
            if (curr.count(d))
                intersection.insert(d);
        }
        result = intersection;
    }

    return vector<int>(result.begin(), result.end());
}

// -------- Boolean OR --------
vector<int> PhraseBooleanEngine::booleanOR(const vector<string>& terms) {
    set<int> result;

    for (const string& t : terms) {
        vector<int> docs = index.getDocuments(t);
        result.insert(docs.begin(), docs.end());
    }

    return vector<int>(result.begin(), result.end());
}

// -------- Boolean NOT --------
vector<int> PhraseBooleanEngine::booleanNOT(const string& term, int totalDocs) {
    set<int> excluded(index.getDocuments(term).begin(),
                      index.getDocuments(term).end());

    vector<int> result;
    for (int i = 0; i < totalDocs; i++) {
        if (!excluded.count(i))
            result.push_back(i);
    }

    return result;
}
