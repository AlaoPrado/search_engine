#include "InvertedIndex.hpp"

namespace search_engine {

InvertedIndex::InvertedIndex(std::vector<Document> &documentList) {
  this->invertedListMap = new std::map<std::string, InvertedList *>();
  this->urlMap = new std::map<std::size_t, std::string>();

  std::size_t documentId = 0;
  for (auto &&document : documentList) {
    this->urlMap->operator[](documentId) = document.getUrl();
    documentId++;
  }
}

InvertedIndex::~InvertedIndex() {
  for (auto it = this->invertedListMap->begin();
       it != this->invertedListMap->end(); it++) {
    delete it->second;
  }
  delete this->invertedListMap;
  delete this->urlMap;
}

} // namespace search_engine
