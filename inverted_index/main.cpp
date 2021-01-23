#include "../utils/Assert.hpp"
#include "../utils/File.hpp"
#include "InvertedIndex.hpp"
#include "document/Document.hpp"
#include "document/DocumentCollection.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

int main(const int argc, const char **argv) {
  try {
    search_engine::utils::assertTrue(
        argc > 1,
        "Error: an collection directory must be passed by parameter.");

    const std::string collectionDirectory = argv[1];
    search_engine::DocumentCollection documentCollection(collectionDirectory);
    std::vector<search_engine::Document> documentList =
        documentCollection.getDocumentList();

    search_engine::utils::fileWrite("document_collection.txt",
                                    documentCollection.toString() + "\n");

    search_engine::InvertedIndex invertedIndex(documentList);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
