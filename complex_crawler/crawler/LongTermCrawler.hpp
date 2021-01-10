#pragma once

#include "Crawler.hpp"
#include "scheduler/PriorityUrlScheduler.hpp"
#include <CkSpider.h>
#include <map>
#include <string>
#include <vector>

namespace search_engine {

class LongTermCrawler : Crawler {
private:
  PriorityUrlScheduler priorityUrlScheduler;

  void pushUrlsIntoScheduler(CkSpider &spider,
                             std::map<std::string, bool> &viewedUrls,
                             std::size_t numPagesToCrawl);

public:
  LongTermCrawler(bool verbose = true);
  virtual void crawl(std::vector<std::string> &seedUrls,
                     std::size_t numPagesToCrawl) override;
};

} // namespace search_engine
