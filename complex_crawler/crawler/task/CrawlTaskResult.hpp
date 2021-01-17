#pragma once

#include "../Page.hpp"
#include <CkSpider.h>

namespace search_engine {

class CrawlTaskResult {
private:
  CkSpider *spider;
  Page page;
  bool success;

public:
  CrawlTaskResult(CkSpider *spider, Page page, bool success);
  CkSpider *getSpider();
  Page getPage();
  bool getSuccess();
};

} // namespace search_engine
