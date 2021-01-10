#pragma once

#include "Crawler.hpp"

namespace search_engine {

class LongTermCrawler : Crawler{
public:
  LongTermCrawler();
  virtual void crawl() override;
};

} // namespace search_engine