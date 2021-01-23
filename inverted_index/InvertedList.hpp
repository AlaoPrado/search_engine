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
  void add(InvertedListEntry entry);
  std::size_t size();
  std::size_t getNumBytes();
};

} // namespace search_engine
