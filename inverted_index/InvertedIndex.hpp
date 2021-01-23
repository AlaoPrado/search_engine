#pragma once

#include "InvertedList.hpp"
#include <cstddef>
#include <map>
#include <string>

namespace search_engine {

class InvertedIndex {
private:
  std::map<std::string, InvertedList *> *invertedListMap;
  std::map<std::size_t, std::string> *urlMap;

public:
  InvertedIndex(std::string collectionDirecory);
  ~InvertedIndex();
};

} // namespace search_engine
