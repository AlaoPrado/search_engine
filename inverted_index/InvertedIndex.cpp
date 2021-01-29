#include "InvertedIndex.hpp"
#include "../html_parser/HtmlParser.hpp"
#include "../text_parser/TextParser.hpp"
#include "../utils/Assert.hpp"
#include <exception>
#include <fstream>
#include <iostream>

namespace search_engine {

void InvertedIndex::addDocument(
    std::size_t documentId,
    std::map<std::string, std::vector<std::size_t>> &occurenceListMap) {
  std::map<std::string, InvertedList *>::iterator invertedListPair;
  for (auto occurenceListPair = occurenceListMap.begin();
       occurenceListPair != occurenceListMap.end(); occurenceListPair++) {
    std::string word = occurenceListPair->first;

    invertedListPair = this->invertedListMap->find(word);

    if (invertedListPair == this->invertedListMap->end()) {
      this->invertedListMap->operator[](word) = new InvertedList();
    }

    auto &&currentInvList = this->invertedListMap->operator[](word);

    currentInvList->add(documentId, occurenceListPair->second.size(),
                        occurenceListPair->second);
  }
}

InvertedIndex::InvertedIndex(std::vector<Document> &documentList) {
  this->invertedListMap = new std::map<std::string, InvertedList *>();
  this->urlMap = new std::map<std::size_t, std::string>();

  std::size_t documentId = 0;
  std::string documentText("");
  for (std::size_t i = 0; i < documentList.size(); i++) {
    try {
      HtmlParser::extractText(documentList[i].getDirectory(), documentText);

      std::map<std::string, std::vector<std::size_t>> occurenceListMap;

      TextParser::extractOccurenceListMap(documentText, occurenceListMap);
      this->urlMap->operator[](documentId) = documentList[i].getUrl();
      this->addDocument(documentId, occurenceListMap);
      documentId++;
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    }
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

std::size_t InvertedIndex::getInvertedListStructureNumBytes() {
  size_t numBytes = sizeof(std::map<std::string, InvertedList *>);
  for (auto it = this->invertedListMap->begin();
       it != this->invertedListMap->end(); it++) {
    numBytes += it->first.size() + it->second->getNumBytes();
  }
  return numBytes;
}

std::size_t InvertedIndex::getUrlMapNumBytes() {
  size_t numBytes = sizeof(this->urlMap);
  for (auto it = this->urlMap->begin(); it != this->urlMap->end(); it++) {
    numBytes += sizeof(std::size_t) + it->second.size();
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

std::size_t InvertedIndex::getAverageInveredListNumOccurences() {
  size_t averageNumOccurences = 0;
  size_t vocabularySize = this->getVocabularySize();

  if (vocabularySize > 0) {
    for (auto it = this->invertedListMap->begin();
         it != this->invertedListMap->end(); it++) {
      averageNumOccurences += it->second->getNumOccurences();
    }

    averageNumOccurences /= vocabularySize;
  }

  return averageNumOccurences;
}

std::vector<std::string> InvertedIndex::getVocabulary() {
  std::vector<std::string> vocabulary;

  for (auto it = this->invertedListMap->begin();
       it != this->invertedListMap->end(); it++) {
    vocabulary.push_back(it->first);
  }

  return vocabulary;
}

void InvertedIndex::store(std::string fileName) {
  std::ofstream file(fileName, std::ofstream::out);

  utils::assertTrue(file.is_open(),
                    "Error(InvertedIndex/store): failed to open file");

  file << this->invertedListMap->size() << std::endl;
  for (auto it = this->invertedListMap->begin();
       it != this->invertedListMap->end(); it++) {
    file << it->first << " " << it->second->size() << std::endl;
    for (size_t i = 0; i < it->second->size(); i++) {
      file << it->second->get(i)->toString() << std::endl;
    }
  }
  file << this->urlMap->size() << std::endl;
  for (auto it = this->urlMap->begin(); it != this->urlMap->end(); it++) {
    file << it->first << " " << it->second << std::endl;
  }
  file.close();
}

} // namespace search_engine
