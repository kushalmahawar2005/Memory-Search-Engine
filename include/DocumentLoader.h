#ifndef DOCUMENT_LOADER_H
#define DOCUMENT_LOADER_H

#include <unordered_map>
#include <string>

class DocumentLoader {
public:
    // File se documents load karega
    void loadDocuments(const std::string& filePath);

    // Loaded documents return karega
    std::unordered_map<int, std::string> getDocuments();

private:
    // DocID -> Document Text
    std::unordered_map<int, std::string> documents;
};

#endif
