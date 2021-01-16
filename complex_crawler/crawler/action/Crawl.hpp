#pragma once

#include "../SiteAttributes.hpp"
#include <CkSpider.h>
#include <chrono>
#include <pthread.h>
#include <string>
#include <vector>
namespace search_engine {

class Crawl {
public:
  typedef std::chrono::milliseconds millis;
  typedef std::chrono::steady_clock::time_point timePoint;

private:
  constexpr static int CRAWL_AWAIT_TIME_MS = 5000;

  Crawl() {}
  static void crawlSleepUntilMs(CkSpider &spider,
                                Crawl::timePoint &lastCrawlEndTime);

public:
  static void crawlUrl(CkSpider &spider, std::string &url,
                       std::vector<std::string> &mustMatchPatterns,
                       std::vector<std::string> &avoidPatterns,
                       double &totalTime, Crawl::timePoint &lastCrawlEndTime,
                       bool useLastCrawlEndTime = false,
                       pthread_mutex_t *memoryMutex = NULL);

  static void crawlUrl(CkSpider &spider, std::string &url,
                       std::vector<std::string> &mustMatchPatterns,
                       std::vector<std::string> &avoidPatterns,
                       SiteAttributes &siteAttributes,
                       Crawl::timePoint &lastCrawlEndTime,
                       bool useLastCrawlEndTime = false,
                       pthread_mutex_t *memoryMutex = NULL);
};

} // namespace search_engine
