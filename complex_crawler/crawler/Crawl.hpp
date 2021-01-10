#pragma once

#include <CkSpider.h>
#include <chrono>

namespace search_engine {

class Crawl {
  typedef std::chrono::milliseconds millis;
  typedef std::chrono::steady_clock::time_point timePoint;

private:
  constexpr static int CRAWL_AWAIT_TIME_MS = 100;

public:
  static void crawlSleepUntilMs(CkSpider &spider,
                               Crawl::timePoint &lastCrawlEndTime,
                               Crawl::timePoint &currentTime);
  static void crawlUrl(CkSpider &spider, std::string &url,
                       Crawl::timePoint &lastCrawlEndTime, double &totalTime);
};

} // namespace search_engine
