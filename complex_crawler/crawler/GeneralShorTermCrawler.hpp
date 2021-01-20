#pragma once

#include "Crawler.hpp"
#include <string>
#include <vector>

namespace search_engine {

class GeneralShorTermCrawler : public Crawler {
private:
  int numThreads;

public:
  GeneralShorTermCrawler(std::string storageDirectory, bool verbose = true,
                   int numThreads = 1);
  ~GeneralShorTermCrawler();
  virtual void crawl(std::vector<std::string> &seedUrls,
                     std::size_t numPagesToCrawl) override;
};

} // namespace search_engine
