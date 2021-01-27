#pragma once

#include "gumbo.h"
#include <string>

namespace search_engine {

class HtmlParser {
private:
  HtmlParser();

  static std::string getTagSeparator(GumboTag tag);
  static bool isTagContentAvoidable(GumboTag tag);
  static std::string cleanText(GumboNode *node);

public:
  static void extractText(const std::string fileDirectory, std::string &text);
};

} // namespace search_engine
