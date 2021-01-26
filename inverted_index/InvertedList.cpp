#include "InvertedList.hpp"

namespace search_engine {

InvertedList::InvertedList() {}

InvertedList::~InvertedList() {}

void InvertedList::add(InvertedListEntry entry) {
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

  return numBytes;
}

} // namespace search_engine
