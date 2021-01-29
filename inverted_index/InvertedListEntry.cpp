#include "InvertedListEntry.hpp"

namespace search_engine {

InvertedListEntry::InvertedListEntry(
    std::size_t documentId, std::size_t numOccurences,
    std::vector<std::size_t> *ocurrenceListBegin)
    : documentId(documentId), numOccurences(numOccurences),
      ocurrenceListBegin(ocurrenceListBegin) {}

const std::size_t *InvertedListEntry::getOcurrenceListBegin() {
  return this->ocurrenceListBegin->data();
}

std::size_t InvertedListEntry::getNumOcurrences() {
  return this->numOccurences;
}

std::size_t InvertedListEntry::getNumBytes() {
  return sizeof(this->documentId) + sizeof(this->numOccurences) +
         sizeof(this->ocurrenceListBegin);
}

std::string InvertedListEntry::toString() {
  std::string s("");

  s += std::to_string(this->documentId) + " " +
       std::to_string(this->numOccurences) + " ";

  size_t i = 0;
  for (; i < this->numOccurences - 1; i++) {
    s += std::to_string(this->ocurrenceListBegin->at(i)) + " ";
  }
  if (i < this->numOccurences) {
    s += std::to_string(this->ocurrenceListBegin->at(i));
  }
  return s;
}

} // namespace search_engine
