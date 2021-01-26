#include "TextParser.hpp"
#include "../utils/Character.hpp"
#include "../utils/TextUtf8.hpp"

namespace search_engine {

void TextParser::extractNextSeparator(const std::string text,
                                      std::size_t beginPosition, bool &isSpace,
                                      std::size_t &endPosition) {
  isSpace = endPosition < text.length();
  for (endPosition = beginPosition; endPosition < text.length();
       endPosition++) {
    if (!utils::isSpacing(text[endPosition]) &&
        !utils::isMisc(text[endPosition])) {
      break;
    }

    isSpace = isSpace && text[endPosition] == ' ';
  }
}

void TextParser::extractNextWord(const std::string text,
                                 std::size_t beginPosition, std::string &word,
                                 std::size_t &endPosition) {
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

void TextParser::extractNextOccurence(
    const std::string text, std::size_t &textPostion,
    std::map<std::string, std::vector<std::size_t>> &occurenceListMap,
    std::size_t &occurencePositon, bool &succcess) {
  std::string word;
  TextParser::extractNextWord(text, textPostion, word, textPostion);
  succcess = word.size() > 0;

  if (succcess) {
    word = utils::textToLowerCaseUtf8(word);
    bool isSpace;

    TextParser::extractNextSeparator(text, textPostion, isSpace, textPostion);

    auto it = occurenceListMap.find(word);

    if (it == occurenceListMap.end()) {
      occurenceListMap.operator[](word) = std::vector<std::size_t>();
    }

    occurenceListMap.operator[](word).push_back(occurencePositon);
    occurencePositon += isSpace ? 1 : 2;
  }
}

void TextParser::extractOccurenceListMap(
    const std::string text,
    std::map<std::string, std::vector<std::size_t>> &occurenceListMap) {
  std::size_t textPosition = 0;
  std::size_t occurencePosition = 0;
  bool success;
  TextParser::extractNextOccurence(text, textPosition, occurenceListMap,
                                   occurencePosition, success);

  while (success) {
    TextParser::extractNextOccurence(text, textPosition, occurenceListMap,
                                     occurencePosition, success);
  }
}

} // namespace search_engine
