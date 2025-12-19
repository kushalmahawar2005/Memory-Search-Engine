#include "DocumentLoader.h"
#include <fstream>
#include <iostream>

void DocumentLoader::loadDocuments(const std::string& filePath) {
    std::ifstream file(filePath);   // file open
    std::string line;
    int docID = 1;                  // document ID start

    while (getline(file, line)) {   // har line read karo
        if (!line.empty()) {
            documents[docID] = line; // map me store
            docID++;                 // next document ID
        }
    }

    file.close(); // file band
}

std::unordered_map<int, std::string> DocumentLoader::getDocuments() {
    return documents;
}
