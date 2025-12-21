# engine_wrapper.py
import search_engine  # pybind11 module

class SearchEngineWrapper:
    def __init__(self):
        self.index = search_engine.InvertedIndex()
        self.ranker = search_engine.RankingEngine(self.index)
        self.trie = search_engine.TrieEngine()
        self.phrase_boolean = search_engine.PhraseBooleanEngine(self.index)

    def add_document(self, doc_id: int, text: str):
        self.index.add_document(doc_id, text)
        self.trie.insert(text)

    def keyword_search(self, query: str, top_k: int):
        return self.ranker.search(query, top_k)

    def phrase_search(self, phrase: str):
        return self.phrase_boolean.phrase_search(phrase)

    def boolean_search(self, query: str):
        return self.phrase_boolean.boolean_search(query)

    def autocomplete(self, prefix: str):
        return self.trie.autocomplete(prefix)
