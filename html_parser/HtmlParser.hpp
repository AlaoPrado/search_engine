#pragma once

#include <string>

namespace search_engine {

class HtmlParser {
private:
  HtmlParser();

  static void skipTagAttributes(const std::string textToParse,
                                std::size_t beginPosition,
                                std::size_t &endPosition);

  static void extractNextTag(const std::string textToParse,
                             std::size_t beginPosition, std::string &tagName,
                             std::size_t &endPosition, bool &open);

  static void skipTagContent(const std::string textToParse,
                             std::size_t beginPosition, std::string tagName,
                             std::size_t &endPosition);

  static void extractTextUntilNextTag(const std::string textToParse,
                                      std::size_t beginPosition,
                                      std::string &text,
                                      std::size_t &endPosition);

public:
  static void extractText(const std::string fileDirectory, std::string &text);
};

} // namespace search_engine
