#pragma once

#include <CkSpider.h>
#include <chrono>
#include <string>
#include <vector>

namespace search_engine {

class Crawl {
private:
  constexpr static int CRAWL_AWAIT_TIME_MS = 100;

public:
  typedef std::chrono::milliseconds millis;
  typedef std::chrono::steady_clock::time_point timePoint;

  static void crawlSleepUntilMs(CkSpider &spider,
                                Crawl::timePoint &lastCrawlEndTime);
  static void crawlUrl(CkSpider &spider, std::string &url,
                       std::vector<std::string> &mustMatchPatterns,
                       std::vector<std::string> &avoidPatterns,
                       double &totalTime, Crawl::timePoint &lastCrawlEndTime,
                       bool useLastCrawlEndTime = false);
};

} // namespace search_engine
