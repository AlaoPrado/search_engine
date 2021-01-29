#include "InvertedList.hpp"

namespace search_engine {

InvertedList::InvertedList() {}

InvertedList::~InvertedList() {}

void InvertedList::add(std::size_t documentId, std::size_t numOccurences,
                       std::vector<std::size_t> &ocurrencePositionList) {

  for (auto &&occurencePosition : ocurrencePositionList) {
    this->ocurrencePositionList.push_back(occurencePosition);
  }

  std::size_t *entryOcurrenceListBegin;
  if (ocurrencePositionList.empty()) {
    entryOcurrenceListBegin = NULL;
  } else {
    entryOcurrenceListBegin = this->ocurrencePositionList.data() +
                              this->ocurrencePositionList.size() -
                              ocurrencePositionList.size();
  }

  InvertedListEntry entry(documentId, numOccurences, entryOcurrenceListBegin);
  this->entriyList.push_back(entry);
}

InvertedListEntry *InvertedList::get(std::size_t index) {
  return &(this->entriyList.operator[](index));
}

std::size_t InvertedList::size() { return this->entriyList.size(); }

std::size_t InvertedList::getNumBytes() {
  std::size_t numBytes = sizeof(this->entriyList);

  for (auto it = this->entriyList.begin(); it != this->entriyList.end(); it++) {
    numBytes += it->getNumBytes();
  }
  numBytes += this->ocurrencePositionList.size() * sizeof(std::size_t) +
              sizeof(this->ocurrencePositionList);
  return numBytes;
}

std::size_t InvertedList::getNumOccurences() {
  return this->ocurrencePositionList.size();
}

} // namespace search_engine
