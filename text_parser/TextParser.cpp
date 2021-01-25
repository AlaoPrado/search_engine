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

void TextParser::extractNextOccurence(const std::string text,
                                      std::size_t &textPostion,
                                      std::vector<Occurrence> &occurenceList,
                                      std::size_t &occurencePositon) {
  std::string word;
  TextParser::extractNextWord(text, textPostion, word, textPostion);

  if (word.size() > 0) {
    word = utils::textToLowerCaseUtf8(word);
    bool isSpace;

    TextParser::extractNextSeparator(text, textPostion, isSpace, textPostion);
    occurenceList.push_back(Occurrence(word, occurencePositon));

    occurencePositon += isSpace ? 1 : 2;
  }
}

void TextParser::extractOccurenceList(const std::string text,
                                      std::vector<Occurrence> &occurenceList) {
  std::size_t textPosition = 0;
  std::size_t occurencePosition = 0;
  std::size_t previousNumOccurences = occurenceList.size();
  TextParser::extractNextOccurence(text, textPosition, occurenceList,
                                   occurencePosition);

  while (previousNumOccurences < occurenceList.size()) {
    previousNumOccurences = occurenceList.size();
    TextParser::extractNextOccurence(text, textPosition, occurenceList,
                                     occurencePosition);
  }
}

} // namespace search_engine
