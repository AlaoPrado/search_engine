#pragma once

#include "InvertedListEntry.hpp"
#include <cstddef>
#include <list>
#include <vector>

namespace search_engine {

class InvertedList {
private:
  std::vector<InvertedListEntry> entriyList;
  std::list<std::vector<size_t>> ocurrencePositionListOfList;

public:
  InvertedList();
  ~InvertedList();
  void add(std::size_t documentId, std::size_t numOccurences,
           std::vector<std::size_t> &ocurrencePositionList);
  InvertedListEntry *get(std::size_t index);
  std::size_t size();
  std::size_t getNumBytes();
  std::size_t getNumOccurences();
};

} // namespace search_engine
