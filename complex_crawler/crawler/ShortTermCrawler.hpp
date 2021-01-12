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
  ShortTermCrawler(int numThreads, std::string storageDirectory,
                   bool verbose = true);
  ~ShortTermCrawler();
  virtual void crawl(std::vector<std::string> &seedUrls,
                     std::size_t numPagesToCrawl) override;
};

} // namespace search_engine
