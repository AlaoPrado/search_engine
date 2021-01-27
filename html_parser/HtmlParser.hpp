#pragma once

#include "gumbo.h"
#include <string>

namespace search_engine {

class HtmlParser {
private:
  HtmlParser();

  static std::string cleantext(GumboNode *node);

public:
  static void extractText(const std::string fileDirectory, std::string &text);
};

} // namespace search_engine
