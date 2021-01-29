#include "../html_parser/HtmlParser.hpp"
#include "../utils/Assert.hpp"
#include "../utils/File.hpp"
#include "InvertedIndex.hpp"
#include "document/Document.hpp"
#include "document/DocumentCollection.hpp"
#include <chrono>
#include <cstddef>
#include <iomanip>
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

    std::chrono::steady_clock::time_point start, end;
    std::chrono::milliseconds duration;

    start = std::chrono::steady_clock::now();
    search_engine::InvertedIndex invertedIndex(documentList);
    end = std::chrono::steady_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::vector<std::string> vocabulary = invertedIndex.getVocabulary();
    std::string vocabularyText("");

    for (auto &&term : vocabulary) {
      vocabularyText += term + "\n";
    }

    search_engine::utils::fileWrite("storage_vocabulary/vocabulary.txt",
                                    vocabularyText);

    const std::size_t bytesPerKBytes = 1000;

    // mini-collection
    std::cout << "Number of documents in the mini-collection: "
              << documentList.size() << std::endl;
    std::cout << std::setprecision(3) << std::fixed;
    std::cout << "Mini-collection size (Kbytes): "
              << (double)documentCollection.getNumBytes() / bytesPerKBytes
              << std::endl;

    // inverted index
    std::cout << std::setprecision(0) << std::fixed;
    std::cout << "Indexing time (Millis): " << duration.count() << std::endl;
    std::cout << std::setprecision(3) << std::fixed;
    std::cout << "Whole inverted index size (Kbytes): "
              << (double)invertedIndex.getNumBytes() / bytesPerKBytes
              << std::endl;
    std::cout << "Size of inverted list structure (Kbytes): "
              << (double)invertedIndex.getInvertedListStructureNumBytes() /
                     bytesPerKBytes
              << std::endl;
    std::cout << "Size of url map (Kbytes): "
              << (double)invertedIndex.getUrlMapNumBytes() / bytesPerKBytes
              << std::endl;

    // vocabulary
    std::cout << std::setprecision(0);
    std::cout << "Vocabulary size (number of distinct terms): "
              << invertedIndex.getVocabularySize() << std::endl;

    // inverted list
    std::cout << "Average number of occurences per term: "
              << invertedIndex.getAverageInveredListNumOccurences()
              << std::endl;
    std::cout << "Average number of documents per term: "
              << invertedIndex.getAverageInvertedListSize() << std::endl;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
