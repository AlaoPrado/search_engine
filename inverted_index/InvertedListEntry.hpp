#pragma once

#include <cstddef>
#include <vector>

namespace search_engine {

class InvertedListEntry {
private:
  std::size_t documentId;
  std::size_t numOccurences;
  std::size_t *ocurrenceListBegin;

public:
  InvertedListEntry(std::size_t documentId, std::size_t numOccurences,
                    std::size_t *ocurrenceListBegin);
  const std::size_t *getOcurrenceListBegin();
  std::size_t getNumOcurrences();
  std::size_t getNumBytes();
};

} // namespace search_engine
