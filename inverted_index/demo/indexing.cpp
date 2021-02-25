/* Copyright 2021 Alan Prado Araújo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
#include "../../utils/Assert.hpp"
#include "../../utils/File.hpp"
#include "../InvertedIndex.hpp"
#include "../document/Document.hpp"
#include "../document/DocumentCollection.hpp"
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
    const std::string storageFolderDirectory =
        argc > 2 ? argv[2] : "../storage/";

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

    std::string indexFileName = storageFolderDirectory + "inverted_index.txt";

    invertedIndex.store(indexFileName);

    std::vector<std::string> vocabulary = invertedIndex.getVocabulary();

    search_engine::utils::fileWrite(
        storageFolderDirectory + "vocabulary.txt",
        invertedIndex.getVocabularyAverageOcurrenceString());

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
    std::cout << "Inverted index file size (Kbytes): "
              << (double)search_engine::utils::fileSize(indexFileName) /
                     bytesPerKBytes
              << std::endl;
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
