#pragma once

#include "Crawler.hpp"
#include <CkSpider.h>
#include <map>
#include <string>
#include <vector>

namespace search_engine {

class LongTermCrawler : public Crawler {  
public:
  LongTermCrawler(bool verbose = true);
  ~LongTermCrawler();
  virtual void crawl(std::vector<std::string> &seedUrls,
                     std::size_t numPagesToCrawl) override;
};

} // namespace search_engine
