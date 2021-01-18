#pragma once

#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include <CkSpider.h>

namespace search_engine {

class CrawlTaskResult {
private:
  CkSpider *spider;
  Page page;
  SiteAttributes *siteAttributes;
  bool success;

public:
  CrawlTaskResult(CkSpider *spider, Page page, SiteAttributes *siteAttributes,
                  bool success);
  CkSpider *getSpider();
  Page getPage();
  SiteAttributes *getSiteAttributes();
  bool getSuccess();
};

} // namespace search_engine
