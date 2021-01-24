#pragma once

#include <string>

namespace search_engine {

class HtmlParser {
private:
  HtmlParser();

public:
  static void readText(const std::string fileDirectory, std::string &text);
};

} // namespace search_engine
