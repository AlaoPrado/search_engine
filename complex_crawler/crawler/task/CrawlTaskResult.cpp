#include "CrawlTaskResult.hpp"
#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include <CkSpider.h>

namespace search_engine {

CrawlTaskResult::CrawlTaskResult(CkSpider *spider, Page page,
                                 SiteAttributes *siteAttributes, bool success)
    : spider(spider), page(page), siteAttributes(siteAttributes),
      success(success) {}

CkSpider *CrawlTaskResult::getSpider() { return this->spider; }

Page CrawlTaskResult::getPage() { return this->page; }

SiteAttributes *CrawlTaskResult::getSiteAttributes() {
  return this->siteAttributes;
}

bool CrawlTaskResult::getSuccess() { return this->success; }

} // namespace search_engine
