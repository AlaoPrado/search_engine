#include "TextParser.hpp"
#include "../utils/Character.hpp"
#include "../utils/Text.hpp"

namespace search_engine {

void TextParser::extractNextWord(const std::string text,
                                 std::size_t beginPosition, std::string &word,
                                 std::size_t &endPosition) {
  endPosition = beginPosition;
  word = "";
  for (endPosition = beginPosition; endPosition < text.length();
       endPosition++) {
    if (utils::isSpacing(text[endPosition]) ||
        utils::isMisc(text[endPosition])) {
      break;
    }

    word += text[endPosition];
  }
}

void TextParser::extractNextOccurence(const std::string text,
                                      std::vector<Occurrence> &occurenceList) {}

void TextParser::extractOccurenceList(const std::string text,
                                      std::vector<Occurrence> &occurenceList) {}

} // namespace search_engine
