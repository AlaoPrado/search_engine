#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace search_engine {

class InvertedListEntry {
private:
  std::size_t documentId;
  std::size_t numOccurences;
  std::vector<std::size_t> *ocurrenceListBegin;

public:
  InvertedListEntry(std::size_t documentId, std::size_t numOccurences,
                    std::vector<std::size_t> *ocurrenceListBegin);
  const std::size_t *getOcurrenceListBegin();
  std::size_t getNumOcurrences();
  std::size_t getNumBytes();
  std::string toString();
};

} // namespace search_engine
