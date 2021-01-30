#include "InvertedIndex.hpp"
#include "../html_parser/HtmlParser.hpp"
#include "../text_parser/TextParser.hpp"
#include "../utils/Assert.hpp"
#include <algorithm>
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <limits>

namespace search_engine {

InvertedIndex::InvertedIndex() {
  this->invertedListMap = new std::map<std::string, InvertedList *>();
  this->urlMap = new std::map<std::size_t, std::string>();
}

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

    auto &&invList = this->invertedListMap->operator[](word);

    invList->add(documentId, occurenceListPair->second.size(),
                 occurenceListPair->second);
  }
}

InvertedIndex::InvertedIndex(std::vector<Document> &documentList)
    : InvertedIndex() {
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

void InvertedIndex::readLineIdAndUrl(std::string line, std::size_t &id,
                                     std::string &url) {
  const std::string separator = " ";
  std::size_t lastSeparatorposition = line.find(separator);

  utils::assertTrue(lastSeparatorposition != std::string::npos &&
                        lastSeparatorposition + 1 < line.length(),
                    "Error(InvertedIndex): invalid input format");

  id = std::stoul(line.substr(0, lastSeparatorposition));
  url = line.substr(lastSeparatorposition + 1);
}

InvertedIndex::InvertedIndex(std::string fileName) : InvertedIndex() {
  std::ifstream file(fileName);

  utils::assertTrue(file.is_open(),
                    "Error(InvertedIndex/constructor): failed to open file");

  std::size_t urlMapSize, documentId, vocabularySize, invListSize,
      numOccurrences;
  std::string line, url, word;

  // url map
  file >> urlMapSize;
  std::cout << urlMapSize << std::endl;
  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (std::size_t i = 0; i < urlMapSize; i++) {
    line = "";
    std::getline(file, line);
    this->readLineIdAndUrl(line, documentId, url);
    this->urlMap->operator[](documentId) = url;
  }

  // inverted file structure
  file >> vocabularySize;
  for (std::size_t i = 0; i < vocabularySize; i++) {
    std::map<std::string, InvertedList *>::iterator invertedListPair;

    file >> word >> invListSize;
    invertedListPair = this->invertedListMap->find(word);
    if (invertedListPair == this->invertedListMap->end()) {
      this->invertedListMap->operator[](word) = new InvertedList();
    }

    auto &&invList = this->invertedListMap->operator[](word);

    for (std::size_t j = 0; j < invListSize; j++) {
      file >> documentId >> numOccurrences;

      std::vector<std::size_t> occurenceList(numOccurrences);

      for (size_t k = 0; k < numOccurrences; k++) {
        file >> occurenceList[k];
      }
      invList->add(documentId, numOccurrences, occurenceList);
    }
  }
  file.close();
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
  std::ofstream file(fileName);

  utils::assertTrue(file.is_open(),
                    "Error(InvertedIndex/store): failed to open file");

  // url map
  file << this->urlMap->size() << std::endl;
  for (auto it = this->urlMap->begin(); it != this->urlMap->end(); it++) {
    file << it->first << " " << it->second << std::endl;
  }

  // inverted file structure
  file << this->invertedListMap->size() << std::endl;
  for (auto it = this->invertedListMap->begin();
       it != this->invertedListMap->end(); it++) {
    file << it->first << " " << it->second->size() << std::endl;
    for (size_t i = 0; i < it->second->size(); i++) {
      file << it->second->get(i)->toString() << std::endl;
    }
  }
  file.close();
}

void InvertedIndex::search(std::string term,
                           std::vector<std::string> &urlList) {
  auto it = this->invertedListMap->find(term);

  if (it != this->invertedListMap->end()) {
    auto &&invList = this->invertedListMap->operator[](term);
    std::size_t documentId;

    for (size_t i = 0; i < std::min(invList->size(), (std::size_t)5); i++) {
      documentId = invList->get(i)->getDocumentId();
      urlList.push_back(this->urlMap->operator[](documentId));
    }
  }
}

} // namespace search_engine
