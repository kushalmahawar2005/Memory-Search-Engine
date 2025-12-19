#ifndef RANKING_ENGINE_H
#define RANKING_ENGINE_H

#include "inverted_index.h"
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class RankingEngine {
private:
    InvertedIndex& index;
    int totalDocuments;

public:
    RankingEngine(InvertedIndex& idx, int totalDocs);

    // TF-IDF ranked keyword search
    vector<pair<int, double>> keywordSearch(
        const vector<string>& queryWords,
        int topK
    );
};

#endif
