#include "CrawlTaskResult.hpp"
#include "../Page.hpp"
#include <CkSpider.h>

namespace search_engine {

CrawlTaskResult::CrawlTaskResult(CkSpider *spider, Page page, bool success)
    : spider(spider), page(page), success(success) {}

CkSpider *CrawlTaskResult::getSpider() { return this->spider; }

Page CrawlTaskResult::getPage() { return this->page; }

bool CrawlTaskResult::getSuccess() { return this->success; }

} // namespace search_engine
