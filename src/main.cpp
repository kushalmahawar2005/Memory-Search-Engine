#include <iostream>
#include "DocumentLoader.h"

int main() {
    DocumentLoader loader;

    // Documents load karo
    loader.loadDocuments("data/documents.txt");

    // Documents nikalo
    auto docs = loader.getDocuments();

    // Print karke check karo
    for (auto& d : docs) {
        std::cout << "DocID " << d.first 
                  << " : " << d.second << std::endl;
    }

    return 0;
}
