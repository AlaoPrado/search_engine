#include "InvertedListEntry.hpp"

namespace search_engine {

InvertedListEntry::InvertedListEntry(std::size_t documentId)
    : documentId(documentId) {}

void InvertedListEntry::add(std::size_t ocurrence) {
  this->ocurrenceList.push_back(ocurrence);
}

std::size_t InvertedListEntry::getNumOcurrences() {
  return this->ocurrenceList.size();
}

std::size_t InvertedListEntry::getNumBytes() {
  return sizeof(this->documentId) +
         sizeof(std::size_t) * this->ocurrenceList.size() +
         sizeof(this->ocurrenceList);
}

} // namespace search_engine
