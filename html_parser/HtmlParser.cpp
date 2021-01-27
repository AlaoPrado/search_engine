#include "HtmlParser.hpp"
#include "../utils/Assert.hpp"
#include "../utils/Character.hpp"
#include "../utils/Text.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace search_engine {

void HtmlParser::skipTagAttributes(const std::string textToParse,
                                   std::size_t beginPosition,
                                   std::size_t &endPosition) {
  const char ATTRIB_ASSIGN_CHAR = '=';
  const char ATTRIB_WRAPPER_CHAR = '\"';
  const char TAG_END_CHAR = '>';

  endPosition = textToParse.find(ATTRIB_ASSIGN_CHAR, beginPosition);

  utils::assertTrue(endPosition != std::string::npos,
                    "Error(HtmlPaser/skipTagAttributes): invalid format");

  endPosition++;

  bool wrapperOpened = false;
  while (endPosition < textToParse.length()) {
    if (!wrapperOpened && textToParse[endPosition] == TAG_END_CHAR) {
      break;
    }

    if (textToParse[endPosition] == ATTRIB_WRAPPER_CHAR) {
      wrapperOpened = !wrapperOpened;
    }

    endPosition++;
  }
}

void HtmlParser::extractNextTag(const std::string textToParse,
                                std::size_t beginPosition, std::string &tagName,
                                std::size_t &endPosition, bool &open) {
  const char TAG_BEGIN_CHAR = '<';
  const char TAG_END_CHAR = '>';
  const char TAG_CLOSE_CHAR = '/';

  tagName = "";
  endPosition = textToParse.find(TAG_BEGIN_CHAR, beginPosition);

  if (endPosition == std::string::npos) {
    return;
  }

  endPosition++;

  open = endPosition < textToParse.length() &&
         textToParse[endPosition] != TAG_CLOSE_CHAR;

  if (!open) {
    endPosition++;
  }

  while (endPosition < textToParse.length()) {
    if (textToParse[endPosition] == TAG_END_CHAR) {
      endPosition++;
      return;
    }

    if (utils::isSpacing(textToParse[endPosition])) {
      endPosition++;
      break;
    }

    tagName += textToParse[endPosition];
    endPosition++;
  }

  HtmlParser::skipTagAttributes(textToParse, endPosition, endPosition);

  utils::assertTrue(textToParse[endPosition] == TAG_END_CHAR,
                    "Error(HtmlPaser/extractNextTag): invalid format");

  endPosition++;
}

void HtmlParser::skipTagContent(const std::string textToParse,
                                std::size_t beginPosition, std::string tagName,
                                std::size_t &endPosition) {
  const std::string TAG_BEGIN_CHAR = "<";
  const std::string TAG_CLOSE_CHAR = "/";
  const std::string tagBegin = " " + TAG_BEGIN_CHAR + tagName;
  const std::string tagEnd = " " + TAG_BEGIN_CHAR + TAG_CLOSE_CHAR + tagName;
  std::size_t currentBegin, internPosition;
  std::string subString;

  do {
    currentBegin = beginPosition;
    endPosition = textToParse.find(tagEnd, currentBegin);

    utils::assertTrue(endPosition != std::string::npos,
                      "Error(HtmlParser/skipTagContent): invalid format");

    subString = textToParse.substr(currentBegin, endPosition);
    internPosition = subString.find(tagBegin);
    endPosition++;
  } while (internPosition != std::string::npos);

  endPosition++;
}

void HtmlParser::extractTextUntilNextTag(const std::string textToParse,
                                         std::size_t beginPosition,
                                         std::string &text,
                                         std::size_t &endPosition) {}

void HtmlParser::extractText(const std::string fileDirectory,
                             std::string &text) {
  std::ifstream file;

  file.open(fileDirectory);

  utils::assertTrue(file.is_open(),
                    "Error(HtmlParser): failed to open file " + fileDirectory);

  std::string textToParse((std::istreambuf_iterator<char>(file)),
                          (std::istreambuf_iterator<char>()));

  file.close();

  textToParse = utils::textToLowerCase(textToParse);
  text = "";

  std::vector<std::string> avoidTagList = {"script"};
  std::string tagName("");
  bool open = false;
  std::size_t endPosition = 0;

  while (endPosition < textToParse.length()) {
    HtmlParser::extractNextTag(textToParse, endPosition, tagName, endPosition,
                               open);
    std::cout << tagName << std::endl;
  }

  text =
      "Esse é um texto de exemplo. Ele tem ponto, vírgula; também tem outros "
      "tipos de pontuação: como ponto e vírgula e dois pontos.";
}

} // namespace search_engine
