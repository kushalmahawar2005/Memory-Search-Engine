"""
Pure Python implementation of search engine
Used as fallback when C++ module is not available
"""

import re
from collections import defaultdict
from math import log, sqrt

class InvertedIndex:
    def __init__(self):
        self.documents = {}
        self.index = defaultdict(lambda: defaultdict(list))
    
    def tokenize(self, text):
        """Tokenize text into words"""
        text = text.lower()
        words = re.findall(r'\w+', text)
        return words
    
    def add_document(self, doc_id, text):
        """Add a document to the index"""
        self.documents[doc_id] = text
        tokens = self.tokenize(text)
        for i, token in enumerate(tokens):
            self.index[token][doc_id].append(i)
    
    def get_postings(self, term):
        """Get postings list for a term"""
        return dict(self.index.get(term, {}))
    
    def doc_count(self):
        """Get total number of documents"""
        return len(self.documents)


class TrieEngine:
    def __init__(self):
        self.trie = {}
        self.words = set()
    
    def insert(self, text):
        """Insert text into trie"""
        tokens = re.findall(r'\w+', text.lower())
        for token in tokens:
            self.words.add(token)
            node = self.trie
            for char in token:
                if char not in node:
                    node[char] = {}
                node = node[char]
    
    def autocomplete(self, prefix):
        """Get autocomplete suggestions"""
        if not prefix:
            return []
        
        suggestions = []
        for word in self.words:
            if word.startswith(prefix):
                suggestions.append(word)
        
        return sorted(suggestions)[:10]


class RankingEngine:
    def __init__(self, inverted_index):
        self.index = inverted_index
    
    def search(self, query, top_k=5):
        """TF-IDF based search"""
        query_tokens = re.findall(r'\w+', query.lower())
        
        if not query_tokens:
            return []
        
        doc_scores = defaultdict(float)
        
        for token in query_tokens:
            postings = self.index.get_postings(token)
            for doc_id in postings:
                # Simple TF-IDF approximation
                tf = len(postings[doc_id])
                idf = log(max(1, self.index.doc_count() / (len(postings) + 1)))
                doc_scores[doc_id] += tf * idf
        
        # Sort by score
        results = sorted(doc_scores.items(), key=lambda x: x[1], reverse=True)
        return results[:top_k]


class PhraseBooleanEngine:
    def __init__(self, inverted_index):
        self.index = inverted_index
    
    def phrase_search(self, phrase):
        """Search for exact phrase"""
        phrase_tokens = re.findall(r'\w+', phrase.lower())
        
        if not phrase_tokens:
            return []
        
        matching_docs = set()
        
        # Get documents containing first word
        first_postings = self.index.get_postings(phrase_tokens[0])
        
        for doc_id in first_postings:
            doc_text = self.index.documents[doc_id].lower()
            phrase_text = ' '.join(phrase_tokens)
            
            if phrase_text in doc_text:
                matching_docs.add(doc_id)
        
        return sorted(list(matching_docs))
    
    def boolean_search(self, query):
        """Boolean search with AND, OR, NOT"""
        # Simple boolean search implementation
        query = query.lower()
        
        matching_docs = set(self.index.documents.keys())
        
        # Handle NOT
        not_terms = re.findall(r'NOT\s+(\w+)', query)
        for term in not_terms:
            postings = self.index.get_postings(term)
            matching_docs -= set(postings.keys())
        
        # Handle AND
        and_terms = re.findall(r'(\w+)\s+AND', query)
        for term in and_terms:
            postings = self.index.get_postings(term)
            matching_docs &= set(postings.keys())
        
        # Handle individual terms
        terms = re.findall(r'\b[a-z]+\b', query.replace('AND', '').replace('OR', '').replace('NOT', ''))
        if terms:
            term_docs = set()
            for term in terms:
                postings = self.index.get_postings(term)
                term_docs |= set(postings.keys())
            matching_docs &= term_docs
        
        return sorted(list(matching_docs))
