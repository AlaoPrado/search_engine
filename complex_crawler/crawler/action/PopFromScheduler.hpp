#pragma once

#include "../SiteAttributes.hpp"
#include "../scheduler/PageScheduler.hpp"
#include "Crawl.hpp"
#include <chrono>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {
class PopFromScheduler {
private:
  PopFromScheduler();

public:
  static void pop(PageScheduler &pageScheduler, std::string &url,
                  std::string &baseUrl, bool &useLastCrawlEndTime,
                  std::map<std::string, SiteAttributes> &siteAttributesMap,
                  std::map<std::string, Crawl::timePoint> &lastCrawlEndTimeMap,
                  pthread_mutex_t *memoryMutex = NULL);
};

} // namespace search_engine
