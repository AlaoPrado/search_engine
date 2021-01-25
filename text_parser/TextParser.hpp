#pragma once

#include "Occurrence.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace search_engine {

class TextParser {
private:
  TextParser();

  static void extractNextSeparator(const std::string text,
                                   std::size_t beginPosition, bool &isSpace,
                                   std::size_t &endPosition);

  static void extractNextWord(const std::string text, std::size_t beginPosition,
                              std::string &word, std::size_t &endPosition);

  static void extractNextOccurence(const std::string text,
                                   std::size_t &textPostion,
                                   std::vector<Occurrence> &occurenceList,
                                   std::size_t &occurencePositon);

public:
  static void extractOccurenceList(const std::string text,
                                   std::vector<Occurrence> &occurenceList);
};

} // namespace search_engine
