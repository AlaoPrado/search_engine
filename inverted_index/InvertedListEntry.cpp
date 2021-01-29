#include "InvertedListEntry.hpp"

namespace search_engine {

InvertedListEntry::InvertedListEntry(std::size_t documentId,
                                     std::size_t numOccurences)
    : documentId(documentId), ocurrencePositionList(numOccurences) {}

void InvertedListEntry::setOccurrence(std::size_t index,
                                     std::size_t ocurrencePosition) {
  this->ocurrencePositionList[index] = ocurrencePosition;
}

std::size_t InvertedListEntry::getOccurrence(std::size_t index) {
  return this->ocurrencePositionList[index];
}

std::size_t InvertedListEntry::getNumOcurrences() {
  return this->ocurrencePositionList.size();
}

std::size_t InvertedListEntry::getNumBytes() {
  return sizeof(this->documentId) +
         sizeof(std::size_t) * this->ocurrencePositionList.size() +
         sizeof(this->ocurrencePositionList);
}

} // namespace search_engine
