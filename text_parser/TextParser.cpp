#include "TextParser.hpp"
#include "../utils/Assert.hpp"
#include "../utils/Character.hpp"
#include "../utils/CharacterUtf8.hpp"
#include "../utils/TextUtf8.hpp"

namespace search_engine {

void TextParser::extractNextSeparator(const std::string text,
                                      std::size_t beginPosition, bool &isSpace,
                                      std::size_t &endPosition) {
  int utf8CharNumBytes;

  isSpace = endPosition < text.length();
  endPosition = beginPosition;

  while (endPosition < text.length()) {
    utf8CharNumBytes = utils::getCharUtf8NumBytes(text[endPosition]);

    if (utf8CharNumBytes == 1) {
      if (!utils::isSpacing(text[endPosition]) &&
          !utils::isMisc(text[endPosition])) {
        break;
      }
      isSpace = isSpace && text[endPosition] == ' ';
      endPosition++;
    } else if (utf8CharNumBytes == 2) {
      unsigned short charUtf2bytes = utils::charUtf8Char2ToUShort(
          text[endPosition], text[endPosition + 1]);

      if (utils::isCharUtf8Latin(charUtf2bytes) &&
          !utils::isCharUtf8LatinMisc(charUtf2bytes)) {
        break;
      }
      isSpace = false;
      endPosition += utf8CharNumBytes;
    } else {
      isSpace = false;
      endPosition += utf8CharNumBytes;
    }
  }
}

void TextParser::extractNextWord(const std::string text,
                                 std::size_t beginPosition, std::string &word,
                                 std::size_t &endPosition) {
  int utf8CharNumBytes;

  word = "";
  endPosition = beginPosition;

  while (endPosition < text.length()) {
    utf8CharNumBytes = utils::getCharUtf8NumBytes(text[endPosition]);

    if (utf8CharNumBytes == 1) {
      if (utils::isSpacing(text[endPosition]) ||
          utils::isMisc(text[endPosition])) {
        break;
      }
      word += text[endPosition];
      endPosition++;
    } else if (utf8CharNumBytes == 2) {
      unsigned short charUtf2bytes = utils::charUtf8Char2ToUShort(
          text[endPosition], text[endPosition + 1]);

      if (!utils::isCharUtf8Latin(charUtf2bytes) ||
          utils::isCharUtf8LatinMisc(charUtf2bytes)) {
        break;
      }
      word += text[endPosition];
      word += text[endPosition + 1];
      endPosition += utf8CharNumBytes;
    } else {
      break;
    }
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
