#pragma once

#include <string>

namespace search_engine {

class UrlEntry {
private:
  std::string url;
  int size;
public:
  UrlEntry(std::string url, int size);
  std::string getUrl();
  int getSize();
};

} // namespace search_engine
