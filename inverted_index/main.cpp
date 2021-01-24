#include "../html_parser/HtmlParser.hpp"
#include "../utils/Assert.hpp"
#include "../utils/File.hpp"
#include "InvertedIndex.hpp"
#include "document/Document.hpp"
#include "document/DocumentCollection.hpp"
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

    search_engine::InvertedIndex invertedIndex(documentList);
    const std::size_t bytesPerKBytes = 1000;

    std::string text;
    search_engine::HtmlParser::readText(documentList[0].getDirectory(), text);
    search_engine::utils::fileWrite("out.html", text);

    std::cout << std::setprecision(3) << std::fixed;
    std::cout << "Whole inverted index size (Kbytes): "
              << invertedIndex.getNumBytes() / bytesPerKBytes << std::endl;
    std::cout << "Size of inverted list structure (Kbytes): "
              << invertedIndex.getInvertedListStructureNumBytes() /
                     bytesPerKBytes
              << std::endl;
    std::cout << "Size of url map (Kbytes): "
              << invertedIndex.getUrlMapNumBytes() / bytesPerKBytes
              << std::endl;

    std::cout << std::setprecision(0);
    std::cout << "Vocabulary size (number of distinct terms): "
              << invertedIndex.getVocabularySize() << std::endl;

    std::cout << "Average number of documents per inverted list: "
              << invertedIndex.getAverageInvertedListSize() << std::endl;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
