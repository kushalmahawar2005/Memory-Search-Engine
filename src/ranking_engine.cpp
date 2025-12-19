#include "ranking_engine.h"
#include <cmath>
#include <queue>
#include <unordered_set>

using namespace std;

RankingEngine::RankingEngine(InvertedIndex& idx, int totalDocs)
    : index(idx), totalDocuments(totalDocs) {}

// Perform TF-IDF ranking for keyword search
vector<pair<int, double>> RankingEngine::keywordSearch(
    const vector<string>& queryWords,
    int topK
) {
    unordered_map<int, double> docScores;

    for (const string& word : queryWords) {
        if (!index.containsWord(word))
            continue;

        vector<int> docs = index.getDocuments(word);
        int df = docs.size();

        if (df == 0) continue;

        double idf = log((double)totalDocuments / df);

        for (int docID : docs) {
            int tf = index.getPositions(word, docID).size();
            docScores[docID] += tf * idf;
        }
    }

    // Max heap for Top-K results
    priority_queue<pair<double, int>> pq;
    for (const auto& entry : docScores) {
        pq.push({entry.second, entry.first});
    }

    vector<pair<int, double>> results;
    while (!pq.empty() && results.size() < topK) {
        auto top = pq.top();
        pq.pop();
        results.push_back({top.second, top.first});
    }

    return results;
}
