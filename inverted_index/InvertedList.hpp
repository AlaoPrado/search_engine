#pragma once

#include "InvertedListEntry.hpp"
#include <cstddef>
#include <vector>

namespace search_engine {

class InvertedList {
private:
  std::vector<InvertedListEntry> entriyList;

public:
  InvertedList();
  ~InvertedList();
  void add(InvertedListEntry entry);
  InvertedListEntry *get(std::size_t index);
  std::size_t size();
  std::size_t getNumBytes();
};

} // namespace search_engine
