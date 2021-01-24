#include "TextParser.hpp"
#include "../utils/Text.hpp"

namespace search_engine {

void TextParser::extractNextWord(const std::string text,
                                 std::size_t beginPosition,
                                 std::string &lastSeparator,
                                 std::string &word) {
  lastSeparator = "";
  word = "";
  for (std::size_t i = beginPosition; i < text.length(); i++) {
  }
}

void TextParser::extractNextOccurence(const std::string text,
                                      std::vector<Occurrence> &occurenceList) {}

void TextParser::extractOccurenceList(const std::string text,
                                      std::vector<Occurrence> &occurenceList) {}

} // namespace search_engine
