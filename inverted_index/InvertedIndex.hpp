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

public:
  InvertedIndex(std::vector<Document> &documentList);
  ~InvertedIndex();
  std::size_t getInvertedListStructureNumBytes();
  std::size_t getUrlMapNumBytes();
  std::size_t getNumBytes();
  std::size_t getVocabularySize();
  std::size_t getAverageInvertedListSize();
};

} // namespace search_engine
