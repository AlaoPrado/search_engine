#pragma once

#include <cstddef>
#include <vector>

namespace search_engine {

class InvertedListEntry {
private:
  std::size_t documentId;
  std::vector<std::size_t> ocurrencePositionList;

public:
  InvertedListEntry(std::size_t documentId);
  void add(std::size_t ocurrencePosition);
  std::size_t getNumOcurrences();
  std::size_t getNumBytes();
};

} // namespace search_engine
