#pragma once

#include "Crawler.hpp"
#include <CkSpider.h>
#include <map>
#include <string>
#include <vector>

namespace search_engine {

class ShortTermCrawler : public Crawler {
private:
  int numThreads;

public:
  ShortTermCrawler(std::string storageDirectory, bool verbose = true,
                   int numThreads = 1);
  ~ShortTermCrawler();
  virtual void crawl(std::vector<std::string> &seedUrls,
                     std::size_t numPagesToCrawl) override;
};

} // namespace search_engine
