#pragma once

#include "../Page.hpp"
#include <string>

namespace search_engine {

class PageEntry {
private:
  Page page;
  int urlSize;

public:
  PageEntry(Page page, int urlSize);
  Page getPage();
  int getUrlSize();
};

} // namespace search_engine
