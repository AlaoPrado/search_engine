#pragma once

#include <string>

namespace search_engine {

class Page {
private:
  std::string url;
  int level;

public:
  Page(std::string url, int level = 0);
  std::string getUrl();
  void setUrl(std::string url);
  int getLevel();
};

} // namespace search_engine
