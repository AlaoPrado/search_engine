#pragma once

#include "Crawler.hpp"
#include <string>
#include <vector>

namespace search_engine {

class GeneralShortTermCrawler : public Crawler {
private:
  int numThreads;

public:
  GeneralShortTermCrawler(std::string storageDirectory, bool verbose = true,
                   int numThreads = 1);
  ~GeneralShortTermCrawler();
  virtual void crawl(std::vector<std::string> &seedUrls,
                     std::size_t numPagesToCrawl) override;
};

} // namespace search_engine
