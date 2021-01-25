#include "InvertedIndex.hpp"
#include "../html_parser/HtmlParser.hpp"
#include "../text_parser/Occurrence.hpp"
#include "../text_parser/TextParser.hpp"
#include <iostream>
#include <vector>

namespace search_engine {

InvertedIndex::InvertedIndex(std::vector<Document> &documentList) {
  this->invertedListMap = new std::map<std::string, InvertedList *>();
  this->urlMap = new std::map<std::size_t, std::string>();

  std::size_t documentId = 0;
  std::string documentText("");
  // for (auto &&document : documentList) {
    this->urlMap->operator[](documentId) = documentList[0].getUrl();
    documentId++;
    HtmlParser::readText(documentList[0].getDirectory(), documentText);

    std::vector<Occurrence> occurenceList;

    TextParser::extractOccurenceList(documentText, occurenceList);

    for (auto &&occurence : occurenceList) {
      std::cout << occurence.getWord() << ", " << occurence.getPosition()
                << std::endl;
    }
  // }
}

InvertedIndex::~InvertedIndex() {
  for (auto it = this->invertedListMap->begin();
       it != this->invertedListMap->end(); it++) {
    delete it->second;
  }
  delete this->invertedListMap;
  delete this->urlMap;
}

std::size_t InvertedIndex::getInvertedListStructureNumBytes() {
  size_t numBytes = sizeof(std::map<std::string, InvertedList *>);
  for (auto it = this->invertedListMap->begin();
       it != this->invertedListMap->end(); it++) {
    numBytes += it->second->getNumBytes();
  }
  return numBytes;
}

std::size_t InvertedIndex::getUrlMapNumBytes() {
  size_t numBytes = sizeof(this->urlMap);
  for (auto it = this->urlMap->begin(); it != this->urlMap->end(); it++) {
    numBytes += it->second.size();
  }
  return numBytes;
}

std::size_t InvertedIndex::getNumBytes() {
  return this->getInvertedListStructureNumBytes() + this->getUrlMapNumBytes();
}

std::size_t InvertedIndex::getVocabularySize() {
  return this->invertedListMap->size();
}

std::size_t InvertedIndex::getAverageInvertedListSize() {
  size_t averageSize = 0;
  size_t vocabularySize = this->getVocabularySize();

  if (vocabularySize > 0) {
    for (auto it = this->invertedListMap->begin();
         it != this->invertedListMap->end(); it++) {
      averageSize += it->second->size();
    }

    averageSize /= vocabularySize;
  }

  return averageSize;
}

} // namespace search_engine
