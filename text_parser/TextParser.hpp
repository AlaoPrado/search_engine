#pragma once

#include "Occurrence.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace search_engine {

class TextParser {
private:
  TextParser();

  static void extractNextWord(const std::string text, std::size_t beginPosition,
                              std::string &word, std::size_t &endPosition);

  static void extractNextOccurence(const std::string text,
                                   std::vector<Occurrence> &occurenceList);

public:
  static void extractOccurenceList(const std::string text,
                                   std::vector<Occurrence> &occurenceList);
};

} // namespace search_engine
