#pragma once

#include <cstddef>
#include <map>
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

  static void extractNextOccurence(
      const std::string text, std::size_t &textPostion,
      std::map<std::string, std::vector<std::size_t>> &occurenceListMap,
      std::size_t &occurencePositon, bool &succcess);

public:
  static void extractOccurenceListMap(
      const std::string text,
      std::map<std::string, std::vector<std::size_t>> &occuoccurenceListMaprenceMap);
};

} // namespace search_engine
