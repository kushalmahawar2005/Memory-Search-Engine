#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <queue>

namespace py = pybind11;
using namespace std;

/* ---------- Tokenizer ---------- */
vector<string> tokenize(const string& text) {
    vector<string> tokens;
    string word;
    stringstream ss(text);
    while (ss >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        tokens.push_back(word);
    }
    return tokens;
}

/* ---------- Inverted Index ---------- */
class InvertedIndex {
public:
    unordered_map<int, string> documents;
    unordered_map<string, unordered_map<int, vector<int>>> index;

    void add_document(int doc_id, const string& text) {
        documents[doc_id] = text;
        auto tokens = tokenize(text);
        for (int i = 0; i < tokens.size(); i++) {
            index[tokens[i]][doc_id].push_back(i);
        }
    }

    unordered_map<int, vector<int>> get_postings(const string& term) {
        if (index.count(term)) return index[term];
        return {};
    }

    int doc_count() const {
        return documents.size();
    }
};

/* ---------- Ranking Engine ---------- */
class RankingEngine {
    InvertedIndex& idx;
public:
    RankingEngine(InvertedIndex& index) : idx(index) {}

    vector<pair<int, double>> search(const string& query, int top_k) {
        unordered_map<int, double> scores;
        auto terms = tokenize(query);
        int N = idx.doc_count();

        for (const auto& term : terms) {
            auto postings = idx.get_postings(term);
            if (postings.empty()) continue;
            double idf = log((double)N / postings.size());
            for (auto& [doc, pos] : postings) {
                scores[doc] += pos.size() * idf;
            }
        }

        priority_queue<pair<double, int>> pq;
        for (auto& [doc, score] : scores)
            pq.push({score, doc});

        vector<pair<int, double>> result;
        while (!pq.empty() && result.size() < top_k) {
            auto [score, doc] = pq.top(); pq.pop();
            result.push_back({doc, score});
        }
        return result;
    }
};

/* ---------- Trie Engine ---------- */
class TrieEngine {
    unordered_set<string> words;
public:
    void insert(const string& text) {
        for (auto& t : tokenize(text))
            words.insert(t);
    }

    vector<string> autocomplete(const string& prefix) {
        vector<string> result;
        for (auto& w : words)
            if (w.rfind(prefix, 0) == 0)
                result.push_back(w);
        return result;
    }
};

/* ---------- Phrase & Boolean ---------- */
class PhraseBooleanEngine {
    InvertedIndex& idx;
public:
    PhraseBooleanEngine(InvertedIndex& index) : idx(index) {}

    vector<int> phrase_search(const string& phrase) {
        auto terms = tokenize(phrase);
        if (terms.empty()) return {};

        auto base = idx.get_postings(terms[0]);
        vector<int> results;

        for (auto& [doc, _] : base) {
            bool ok = true;
            for (int i = 1; i < terms.size(); i++) {
                if (!idx.get_postings(terms[i]).count(doc)) {
                    ok = false;
                    break;
                }
            }
            if (ok) results.push_back(doc);
        }
        return results;
    }

    vector<int> boolean_search(const string& query) {
        auto terms = tokenize(query);
        unordered_set<int> res;
        bool first = true;
        string op = "and";

        for (auto& t : terms) {
            if (t == "and" || t == "or" || t == "not") {
                op = t;
                continue;
            }

            unordered_set<int> docs;
            for (auto& [doc, _] : idx.get_postings(t))
                docs.insert(doc);

            if (first) {
                res = docs;
                first = false;
            } else {
                unordered_set<int> temp;
                if (op == "and") {
                    for (int d : res) if (docs.count(d)) temp.insert(d);
                } else if (op == "or") {
                    temp = res;
                    temp.insert(docs.begin(), docs.end());
                } else if (op == "not") {
                    temp = res;
                    for (int d : docs) temp.erase(d);
                }
                res = temp;
            }
        }
        return vector<int>(res.begin(), res.end());
    }
};

/* ---------- PYBIND MODULE ---------- */
PYBIND11_MODULE(search_engine, m) {
    py::class_<InvertedIndex>(m, "InvertedIndex")
        .def(py::init<>())
        .def("add_document", &InvertedIndex::add_document);

    py::class_<RankingEngine>(m, "RankingEngine")
        .def(py::init<InvertedIndex&>())
        .def("search", &RankingEngine::search);

    py::class_<TrieEngine>(m, "TrieEngine")
        .def(py::init<>())
        .def("insert", &TrieEngine::insert)
        .def("autocomplete", &TrieEngine::autocomplete);

    py::class_<PhraseBooleanEngine>(m, "PhraseBooleanEngine")
        .def(py::init<InvertedIndex&>())
        .def("phrase_search", &PhraseBooleanEngine::phrase_search)
        .def("boolean_search", &PhraseBooleanEngine::boolean_search);
}
