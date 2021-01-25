#include "InvertedListEntry.hpp"

namespace search_engine {

InvertedListEntry::InvertedListEntry(std::size_t documentId)
    : documentId(documentId) {}

void InvertedListEntry::add(std::size_t ocurrencePosition) {
  this->ocurrencePositionList.push_back(ocurrencePosition);
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
