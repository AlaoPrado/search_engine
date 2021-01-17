#pragma once

#include "../Page.hpp"
#include <string>

namespace search_engine {

class PageEntry {
private:
  Page page;
  int size;

public:
  PageEntry(Page page, int size);
  Page getPage();
  int getSize();
};

} // namespace search_engine
