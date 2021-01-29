#pragma once

#include "InvertedList.hpp"
#include "document/Document.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace search_engine {

class InvertedIndex {
private:
  std::map<std::string, InvertedList *> *invertedListMap;
  std::map<std::size_t, std::string> *urlMap;

  void
  addDocument(std::size_t documentId,
              std::map<std::string, std::vector<std::size_t>> &occurenceMap);

public:
  InvertedIndex(std::vector<Document> &documentList);
  ~InvertedIndex();
  std::size_t getInvertedListStructureNumBytes();
  std::size_t getUrlMapNumBytes();
  std::size_t getNumBytes();
  std::size_t getVocabularySize();
  std::size_t getAverageInvertedListSize();
  std::size_t getAverageInveredListNumOccurences();
  std::vector<std::string> getVocabulary();
};

} // namespace search_engine
