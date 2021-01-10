#pragma once

#include "Crawler.hpp"
#include "scheduler/PriorityUrlScheduler.hpp"

namespace search_engine {

class LongTermCrawler : Crawler {
private:
  PriorityUrlScheduler priorityUrlScheduler;

public:
  LongTermCrawler();
  virtual void crawl() override;
};

} // namespace search_engine
