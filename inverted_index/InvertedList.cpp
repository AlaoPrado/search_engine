#include "InvertedList.hpp"

namespace search_engine {

InvertedList::InvertedList() {}

void InvertedList::add(InvertedListEntry entry) {
  this->entriyList.push_back(entry);
}

std::size_t InvertedList::size() { return this->entriyList.size(); }

std::size_t InvertedList::getNumBytes() {
  std::size_t numBytes = sizeof(this->entriyList);

  for (auto &&entry : this->entriyList) {
    numBytes += entry.getNumBytes();
  }

  return numBytes;
}

} // namespace search_engine
