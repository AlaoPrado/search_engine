#include "InvertedListEntry.hpp"

namespace search_engine {

InvertedListEntry::InvertedListEntry(std::size_t documentId,
                                     std::size_t numOccurences,
                                     std::size_t *ocurrenceListBegin)
    : documentId(documentId), numOccurences(numOccurences),
      ocurrenceListBegin(ocurrenceListBegin) {}

const std::size_t *InvertedListEntry::getOcurrenceListBegin() {
  return this->ocurrenceListBegin;
}

std::size_t InvertedListEntry::getNumOcurrences() {
  return this->numOccurences;
}

std::size_t InvertedListEntry::getNumBytes() {
  return sizeof(this->documentId) + sizeof(this->numOccurences) +
         sizeof(this->ocurrenceListBegin);
}

} // namespace search_engine
